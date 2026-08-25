KDIR := /lib/modules/$(shell uname -r)/build M=$(PWD)

obj-m += proc_module.o

all:
	make -C $(KDIR) modules

clean:
	make -C $(KDIR) clean
