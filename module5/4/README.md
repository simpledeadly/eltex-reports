# Задание №4

## Сикаченко Дмитрий Константинович

**Окружение:** Arch Linux, ядро `7.1.9-arch1-2`

---

## Описание

Модуль регистрирует символьное устройство `/dev/chardev`. Userspace читает и записывает данные через стандартные `open`/`read`/`write`/`close`. Устройство работает в режиме exclusive open: второй `open` до `close` первого вернёт `EBUSY`.

---

## 1. Сборка модуля

```bash
make

make -C /lib/modules/7.1.9-arch1-2/build M=/home/smpdl/dev/eltex-reports/module5/4/module modules
make[1]: Entering directory '/usr/lib/modules/7.1.9-arch1-2/build'
make[2]: Entering directory '/home/smpdl/dev/eltex-reports/module5/4/module'
  CC [M]  chardev.o
  MODPOST Module.symvers
  CC [M]  chardev.mod.o
  CC [M]  .module-common.o
  LD [M]  chardev.ko
  BTF [M] chardev.ko
make[2]: Leaving directory '/home/smpdl/dev/eltex-reports/module5/4/module'
make[1]: Leaving directory '/usr/lib/modules/7.1.9-arch1-2/build'
```

---

## 2. Загрузка модуля

```bash
sudo insmod chardev.ko
ls -l /dev/chardev

crw-rw-rw- 1 root root 235, 0 Aug 27 20:38 /dev/chardev
```

---

## 3. Проверка работы

```bash
cat /dev/chardev
hello from kernel

echo "test" > /dev/chardev

cat /dev/chardev
test
```

---

## 4. Автоматический тест

```bash
chmod +x ../test.sh
../test.sh

==> Build
    [OK] chardev.ko built

==> Install udev rule (MODE=0666)
    [OK] /etc/udev/rules.d/99-chardev.rules installed

==> Load
    [OK] module loaded

==> Device node exists
    [OK] /dev/chardev exists

==> Device permissions
    mode: 666
    [OK] permissions 666

==> Kernel log after insmod
[ 4601.451450] chardev: registered, major=235
[ 4601.525114] chardev: unregistered
[ 4642.462066] chardev: registered, major=235

==> Read default message
    got: 'hello from kernel'
    [OK] default message correct

==> Write new message
    [OK] write done

==> Read back written message
    got: 'ping from userspace'
    [OK] readback correct

==> Exclusive open: second reader returns EBUSY
    [OK] EBUSY returned as expected

==> Unload
    [OK] module unloaded

==> Device node gone
    [OK] /dev/chardev removed

==> Kernel log after rmmod
[ 4601.525114] chardev: unregistered
[ 4642.462066] chardev: registered, major=235
[ 4642.537951] chardev: unregistered

All tests passed.
```

---

## 5. Выгрузка

```bash
sudo rmmod chardev
ls /dev/chardev

ls: cannot access '/dev/chardev': No such file or directory
```
