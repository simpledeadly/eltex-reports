# Задание №5

## Сикаченко Дмитрий Константинович

**Окружение:** Arch Linux, ядро `7.1.9-arch1-2`

---

## Описание

Модуль создаёт netlink-сокет с протоколом `31`. Userspace-клиент отправляет строку, ядро логирует её и отвечает `"hello from kernel"`. Общение без VFS — напрямую через сетевой стек (`AF_NETLINK`).

---

## 1. Сборка

```bash
# Собрать модуль
make -C module

# Собрать userspace-клиент
gcc -O2 -Wall -o user/nl_client user/nl_client.c
```

---

## 2. Загрузка модуля

```bash
sudo insmod module/netlink_mod.ko
```

---

## 3. Проверка работы

```bash
./user/nl_client

sending: hello from userspace
received: hello from kernel
```

Лог ядра:

```bash
sudo dmesg | grep netlink

[...] netlink: socket created, proto=31
[...] netlink: received from pid 12345: hello from userspace
```

---

## 4. Автоматический тест

```bash
chmod +x test.sh
sudo test.sh

==> Build kernel module
    [OK] netlink_mod.ko built

==> Build userspace client
    [OK] nl_client built

==> Load module
    [OK] module loaded

==> Kernel log after insmod
[ 7912.545374] netlink: received from pid 17444: hello from userspace
[ 7912.587901] netlink: socket released
[ 8085.965940] netlink: socket created, proto=31

==> Send message and receive reply
    sending: hello from userspace
received: hello from kernel
    [OK] reply matches expected

==> Kernel log: received message logged
[ 7912.545374] netlink: received from pid 17444: hello from userspace
[ 8085.994990] netlink: received from pid 18253: hello from userspace

==> Unload module
    [OK] module unloaded

==> Kernel log after rmmod
[ 8085.965940] netlink: socket created, proto=31
[ 8085.994990] netlink: received from pid 18253: hello from userspace
[ 8086.039724] netlink: socket released

All tests passed.
```

---

## 5. Выгрузка

```bash
sudo rmmod netlink_mod
```
