# Задание №6

## Сикаченко Дмитрий Константинович

**Окружение:** Arch Linux, ядро `7.1.9-arch1-2`

---

## Описание

Модуль регистрирует хук `NF_INET_LOCAL_OUT` в netfilter. Если destination IP исходящего пакета есть в чёрном списке, то пакет дропается (`NF_DROP`). Управление списком происходит через `/proc/ipblock/blacklist`:

| Команда | Действие |
| --------- | ---------- |
| `echo "+1.2.3.4" \| sudo tee /proc/ipblock/blacklist` | добавить IP |
| `echo "-1.2.3.4" \| sudo tee /proc/ipblock/blacklist` | удалить IP |
| `cat /proc/ipblock/blacklist` | список заблокированных |

---

## 1. Сборка

```bash
make

make -C /lib/modules/7.1.9-arch1-2/build M=/home/smpdl/dev/eltex-reports/module5/6/module modules
make[1]: Entering directory '/usr/lib/modules/7.1.9-arch1-2/build'
make[2]: Entering directory '/home/smpdl/dev/eltex-reports/module5/6/module'
  CC [M]  ipblock.o
  MODPOST Module.symvers
  CC [M]  ipblock.mod.o
  CC [M]  .module-common.o
  LD [M]  ipblock.ko
  BTF [M] ipblock.ko
make[2]: Leaving directory '/home/smpdl/dev/eltex-reports/module5/6/module'
make[1]: Leaving directory '/usr/lib/modules/7.1.9-arch1-2/build'
```

---

## 2. Загрузка

```bash
sudo insmod ipblock.ko
```

---

## 3. Проверка работы

```bash
echo "+127.0.0.2" | sudo tee /proc/ipblock/blacklist

sudo cat /proc/ipblock/blacklist
127.0.0.2

ping -c 1 -W 1 127.0.0.2
# нет ответа — пакет дропнут

sudo dmesg | grep ipblock
[...] ipblock: dropped packet to 127.0.0.2

echo "-127.0.0.2" | sudo tee /proc/ipblock/blacklist

ping -c 1 127.0.0.2
# 1 packets transmitted, 1 received
```

---

## 4. Автоматический тест

```bash
chmod +x ../test.sh
sudo ../test.sh

==> Build
    [OK] ipblock.ko built

==> Load
    [OK] module loaded

==> Kernel log after insmod
[ 9012.855542] ipblock: dropped packet to 127.0.0.2
[ 9038.140661] ipblock: unloaded
[ 9060.387027] ipblock: loaded, manage via /proc/ipblock/blacklist

==> proc entry exists
    [OK] /proc/ipblock/blacklist exists

==> Blacklist is empty
    [OK] empty

==> Add 127.0.0.2 to blacklist
    [OK] write done

==> List shows 127.0.0.2
    127.0.0.2
    [OK] 127.0.0.2 in list

==> Packet to 127.0.0.2 is dropped
    [OK] ping failed — packet dropped as expected

==> Kernel log: drop recorded
[ 9012.855542] ipblock: dropped packet to 127.0.0.2
[ 9060.466922] ipblock: dropped packet to 127.0.0.2

==> Remove 127.0.0.2 from blacklist
    [OK] write done

==> Blacklist empty again
    [OK] empty

==> Packet to 127.0.0.2 passes now
    [OK] ping succeeded — packet passed

==> Unload
    [OK] module unloaded

==> proc entry gone
    [OK] /proc/ipblock/blacklist removed

==> Kernel log after rmmod
[ 9060.387027] ipblock: loaded, manage via /proc/ipblock/blacklist
[ 9060.466922] ipblock: dropped packet to 127.0.0.2
[ 9061.551510] ipblock: unloaded

All tests passed.
```

---

## 5. Выгрузка

```bash
sudo rmmod ipblock
ls /proc/ipblock

ls: cannot access '/proc/ipblock': No such file or directory
```
