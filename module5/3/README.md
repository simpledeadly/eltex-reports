# Задание №3

## Сикаченко Дмитрий Константинович

**Окружение:** Debian 13, ядро `6.12.86+deb13-amd64`

---

## 1. Сборка модуля

```bash
make

make -C /lib/modules/6.12.86+deb13-amd64/build M=/home/simpledeadly/Documents/dev/eltex/module5/3/module modules
make[1]: Entering directory '/usr/src/linux-headers-6.12.86+deb13-amd64'
  CC [M]  /home/simpledeadly/Documents/dev/eltex/module5/3/module/blink_module.o
  MODPOST /home/simpledeadly/Documents/dev/eltex/module5/3/module/Module.symvers
  CC [M]  /home/simpledeadly/Documents/dev/eltex/module5/3/module/blink_module.mod.o
  CC [M]  /home/simpledeadly/Documents/dev/eltex/module5/3/module/.module-common.o
  LD [M]  /home/simpledeadly/Documents/dev/eltex/module5/3/module/blink_module.ko
  BTF [M] /home/simpledeadly/Documents/dev/eltex/module5/3/module/blink_module.ko
make[1]: Leaving directory '/usr/src/linux-headers-6.12.86+deb13-amd64'
```

---

## 2. Загрузка модуля

```bash
sudo insmod blink_module.ko
```

Проверка sysfs-файла:

```bash
ls /sys/kernel/blink/

state
```

---

## 3. Проверка работы

```bash
echo 0 | sudo tee /sys/kernel/blink/state # сброс
echo 1 | sudo tee /sys/kernel/blink/state # Scroll Lock
echo 2 | sudo tee /sys/kernel/blink/state # Num Lock
echo 4 | sudo tee /sys/kernel/blink/state # Caps Lock
echo 7 | sudo tee /sys/kernel/blink/state # все биты маски
```

**Результат:** при маске `4` и `7` физически загорался индикатор Caps Lock. При масках `1` и `2` видимого эффекта не было, т.к. у используемой клавиатуры физически отсутствуют светодиоды для Num Lock и Scroll Lock. `0` гасил все лампочки.

Дальнейшее тестирование проводилось по логам, так как в процессе работы физически отвалился USB Type-C разъём питания клавиатуры. Других клавиатур нет...

PS: я купил клавиатуру, поэтому демке-таки быть!

![gif_alt](./fig/output.gif)

---

## 4. Подтверждение через журнал ядра `dmesg`

```bash
echo 4 | sudo tee /sys/kernel/blink/state
sudo dmesg | tail -5

[ 1936.126840] blink_module: KDSETLED mask=4, ioctl returned 0
[ 1936.638841] blink_module: KDSETLED mask=0, ioctl returned 0
[ 1937.150821] blink_module: KDSETLED mask=4, ioctl returned 0
[ 1937.662846] blink_module: KDSETLED mask=0, ioctl returned 0
[ 1938.174858] blink_module: KDSETLED mask=4, ioctl returned 0
```

`ioctl` вызывается с переданной маской и завершается успешно с `returned 0`.

---

## 5. Выгрузка модуля

```bash
sudo rmmod blink_module
ls /sys/kernel/blink/

ls: cannot access '/sys/kernel/blink/': No such file or directory
```

---

## 6. Очистка сборки

```bash
make clean

make -C /lib/modules/6.12.86+deb13-amd64/build M=/home/simpledeadly/Documents/dev/eltex/module5/3/module clean
make[1]: Entering directory '/usr/src/linux-headers-6.12.86+deb13-amd64'
  CLEAN   /home/simpledeadly/Documents/dev/eltex/module5/3/module/Module.symvers
make[1]: Leaving directory '/usr/src/linux-headers-6.12.86+deb13-amd64'
```
