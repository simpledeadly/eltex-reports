KDIR := /lib/modules/$(shell uname -r)/build M=$(PWD)
# M=$(PWD) нужно чтобы собрался модуль в указанном каталоге, а не ядро целиком.

obj-m += hmodule.o # объект компилируется как загружаемый .ko модуль.

all:
	make -C $(KDIR) modules
# -C значит что процессу нужно выполнить Makefile, лежащий по пути /lib/modules/$(uname -r)/build, версия ядра: 7.0.0-28-generic

clean:
	make -C $(KDIR) clean
