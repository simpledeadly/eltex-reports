#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chardev"
#define BUF_SIZE 256

static int major;
static char msg[BUF_SIZE] = "hello from kernel\n";
static size_t msg_len;

static struct class *chardev_class;
static struct device *chardev_dev;

static atomic_t is_open = ATOMIC_INIT(0);

static char *chardev_devnode(const struct device *dev, umode_t *mode) {
  if (mode)
    *mode = 0666;
  return NULL;
}

static int chardev_open(struct inode *inode, struct file *file) {
  if (atomic_xchg(&is_open, 1))
    return -EBUSY;
  return 0;
}

static int chardev_release(struct inode *inode, struct file *file) {
  atomic_set(&is_open, 0);
  return 0;
}

static ssize_t chardev_read(struct file *file, char __user *buf, size_t len,
                            loff_t *offset) {
  size_t available = msg_len - *offset;

  if (*offset >= msg_len)
    return 0;

  if (len > available)
    len = available;

  if (copy_to_user(buf, msg + *offset, len))
    return -EFAULT;

  *offset += len;
  return len;
}

static ssize_t chardev_write(struct file *file, const char __user *buf,
                             size_t len, loff_t *offset) {
  if (len >= BUF_SIZE)
    return -EINVAL;

  if (copy_from_user(msg, buf, len))
    return -EFAULT;

  msg[len] = '\0';
  msg_len = len;
  return len;
}

static const struct file_operations chardev_fops = {
    .open = chardev_open,
    .release = chardev_release,
    .read = chardev_read,
    .write = chardev_write,
};

static int __init chardev_init(void) {
  major = register_chrdev(0, DEVICE_NAME, &chardev_fops);
  if (major < 0)
    return major;

  chardev_class = class_create(DEVICE_NAME);
  if (IS_ERR(chardev_class)) {
    unregister_chrdev(major, DEVICE_NAME);
    return PTR_ERR(chardev_class);
  }
  chardev_class->devnode = chardev_devnode;

  chardev_dev =
      device_create(chardev_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
  if (IS_ERR(chardev_dev)) {
    class_destroy(chardev_class);
    unregister_chrdev(major, DEVICE_NAME);
    return PTR_ERR(chardev_dev);
  }

  msg_len = strlen(msg);
  pr_info("%s: registered, major=%d\n", DEVICE_NAME, major);
  return 0;
}

static void __exit chardev_exit(void) {
  device_destroy(chardev_class, MKDEV(major, 0));
  class_destroy(chardev_class);
  unregister_chrdev(major, DEVICE_NAME);
  pr_info("%s: unregistered\n", DEVICE_NAME);
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dmitriy Sikachenko");
MODULE_DESCRIPTION("Bidirectional char device: read/write via /dev/chardev");
