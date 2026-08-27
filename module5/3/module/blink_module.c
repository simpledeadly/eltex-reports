#include <linux/init.h>
#include <linux/kd.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/timer.h>
#include <linux/tty.h>
#include <linux/vt.h>
#include <linux/vt_kern.h>

#define BLINK_INTERVAL msecs_to_jiffies(500)
#define KOBJECT_NAME "blink"
#define LED_RESTORE_DEFAULT 0xFF

static struct tty_driver *my_driver;
static struct kobject *led_kobject;
static int current_mask;

static struct timer_list blink_timer;
static int led_on;

static void set_kbd_leds_raw(int mask) {
  int ret =
      (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED, mask);
  printk(KERN_INFO "blink_module: KDSETLED mask=%d, ioctl returned %d\n", mask,
         ret);
}

static void blink_timer_callback(struct timer_list *t) {
  led_on = !led_on;
  set_kbd_leds_raw(led_on ? current_mask : 0);
  mod_timer(&blink_timer, jiffies + BLINK_INTERVAL);
}

static ssize_t state_show(struct kobject *kobj, struct kobj_attribute *attr,
                          char *buf) {
  return sprintf(buf, "%d\n", current_mask);
}

static ssize_t state_store(struct kobject *kobj, struct kobj_attribute *attr,
                           const char *buf, size_t count) {
  int value;

  if (kstrtoint(buf, 10, &value) < 0)
    return -EINVAL;

  if (value < 0 || value > 7)
    return -EINVAL;

  current_mask = value;

  if (value == 0) {
    timer_delete_sync(&blink_timer);
    led_on = 0;
    set_kbd_leds_raw(0);
  } else {
    led_on = 1;
    set_kbd_leds_raw(current_mask);
    mod_timer(&blink_timer, jiffies + BLINK_INTERVAL);
  }

  return count;
}

static struct kobj_attribute state_attribute =
    __ATTR(state, 0660, state_show, state_store);

static int __init blink_init(void) {
  int error;

  my_driver = vc_cons[fg_console].d->port.tty->driver;

  led_kobject = kobject_create_and_add(KOBJECT_NAME, kernel_kobj);
  if (!led_kobject)
    return -ENOMEM;

  error = sysfs_create_file(led_kobject, &state_attribute.attr);
  if (error) {
    kobject_put(led_kobject);
    return error;
  }

  timer_setup(&blink_timer, blink_timer_callback, 0);
  return 0;
}

static void __exit blink_cleanup(void) {
  timer_delete_sync(&blink_timer);
  set_kbd_leds_raw(LED_RESTORE_DEFAULT);
  kobject_put(led_kobject);
}

module_init(blink_init);
module_exit(blink_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dmitriy Sikachenko");
MODULE_DESCRIPTION("Keyboard LED control via sysfs mask");