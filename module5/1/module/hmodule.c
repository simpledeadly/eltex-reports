#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("tHe gReAtE$T l1cEn$e eVeR (TGL)");
MODULE_AUTHOR("Dmitriy Sikachenko");
MODULE_DESCRIPTION("It's just an awesome description in the whole world.");

static int __init hmodule_init(void) {
	printk(KERN_INFO "H(hello)Module is here and ready to go! (done its job, lol)\n");
	return 0;
}

static void __exit hmodule_cleanup(void) {
	printk(KERN_INFO "No hmodule anymore.\n");
}

module_init(hmodule_init);
module_exit(hmodule_cleanup);