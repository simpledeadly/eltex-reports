#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define MAX_SIZE 10
#define PROC_NAME "quasi_file"

static size_t count_bytes;
static char *msg;

static ssize_t proc_read(struct file *filp, char __user *buf, size_t count,
                         loff_t *offp) {
  if (*offp > 0 || count_bytes == 0)
    return 0;

  if (count > count_bytes)
    count = count_bytes;

  if (copy_to_user(buf, msg, count))
    return -EFAULT;

  *offp += count;
  return count;
}

static ssize_t proc_write(struct file *filp, const char __user *buf,
                          size_t count, loff_t *offp) {
  size_t to_copy = count;

  if (to_copy > MAX_SIZE)
    to_copy = MAX_SIZE;

  if (copy_from_user(msg, buf, to_copy))
    return -EFAULT;

  count_bytes = to_copy;
  return count;
}

static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

static int __init proc_init(void) {
  proc_create(PROC_NAME, 0, NULL, &proc_fops);
  msg = kmalloc(MAX_SIZE, GFP_KERNEL);
  if (!msg)
    return -ENOMEM;
  return 0;
}

static void __exit proc_cleanup(void) {
  remove_proc_entry(PROC_NAME, NULL);
  kfree(msg);
}

module_init(proc_init);
module_exit(proc_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fantomas");
MODULE_AUTHOR("smpdl");
MODULE_DESCRIPTION("Second task yoyoyoyoooo");