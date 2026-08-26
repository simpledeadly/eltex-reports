# Задание №2

## Сикаченко Дмитрий Константинович

**Окружение:** Debian 13, ядро `6.12.86+deb13-amd64`

---

## 1. Установка необходимых заголовков

Заголовки ядра уже были установлены при выполнении задания 1 (`build-essential`, `linux-headers-$(uname -r)`):

```bash
sudo apt install -y build-essential linux-headers-$(uname -r)
```

---

## 2. Модуль ядра `./module/proc_module.c`

Модуль реализует обмен строкой между userspace и ядром через файл `/proc/quasi_file`: запись сохраняет строку (обрезая её до `MAX_SIZE` байт), чтение возвращает то, что было сохранено последним.

---

## 3. Сборка модуля

```bash
make

make -C /lib/modules/6.12.86+deb13-amd64/build M=/home/simpledeadly/Documents/dev/eltex/module5/2/module modules
make[1]: Entering directory '/usr/src/linux-headers-6.12.86+deb13-amd64'
  CC [M]  /home/simpledeadly/Documents/dev/eltex/module5/2/module/proc_module.o
  MODPOST /home/simpledeadly/Documents/dev/eltex/module5/2/module/Module.symvers
  CC [M]  /home/simpledeadly/Documents/dev/eltex/module5/2/module/proc_module.mod.o
  CC [M]  /home/simpledeadly/Documents/dev/eltex/module5/2/module/.module-common.o
  LD [M]  /home/simpledeadly/Documents/dev/eltex/module5/2/module/proc_module.ko
  BTF [M] /home/simpledeadly/Documents/dev/eltex/module5/2/module/proc_module.ko
make[1]: Leaving directory '/usr/src/linux-headers-6.12.86+deb13-amd64'
```

---

## 4. Загрузка модуля

```bash
sudo insmod proc_module.ko
```

Проверка, что файл появился в `/proc`:

```bash
ls /proc | grep quasi_file
quasi_file
```

---

## 5. Ручное тестирование через shell

Запись и чтение строки:

```bash
echo "hi" | sudo tee /proc/quasi_file
cat /proc/quasi_file
hi
```

Проверка обрезки по `MAX_SIZE` (запись строки длиннее лимита):

```bash
echo "HEY, BRUDDAH" | sudo tee /proc/quasi_file
cat /proc/quasi_file
HEY, BRUDD
```

---


## 6. Выгрузка модуля

```bash
sudo rmmod proc_module
ls /proc | grep quasi_file
```

---

## 7. Очистка сборки

```bash
make clean

make -C /lib/modules/6.12.86+deb13-amd64/build M=/home/simpledeadly/Documents/dev/eltex/module5/2/module clean
make[1]: Entering directory '/usr/src/linux-headers-6.12.86+deb13-amd64'
  CLEAN   /home/simpledeadly/Documents/dev/eltex/module5/2/module/Module.symvers
make[1]: Leaving directory '/usr/src/linux-headers-6.12.86+deb13-amd64'
```

---

## 8. Рубрика эксперименты

**Что будет, если `proc_write` при обрезке данных возвращать не изначальный `count`, а реально скопированное (обрезанное) количество байт?**

Первая версия функции обрезала сам параметр `count` и возвращала его же:

```c
if (count > MAX_SIZE)
    count = MAX_SIZE;
...
return count;
```

Проверка через `tee`:

```
$ echo "HEY, BRUDDAH" | sudo tee /proc/quasi_file
HEY, BRUDDAH
$ cat /proc/quasi_file
AH
```

Вместо ожидаемого `"HEY, BRUDD"` (первые 10 байт) в файле оказалось `"AH"`.

**Разбор:** `write()` в POSIX обязан либо записать все запрошенные байты, либо явно сообщить, сколько записано, чтобы вызывающая сторона дозаписала остаток. `tee` следует этому контракту: если `write()` вернул `10` при запросе `12` байт, `tee` интерпретирует это как «записано не всё» и сам довызывает `write()` с оставшимися 2 байтами (`"AH"`). Эти 2 байта копируются в начало буфера поверх уже лежащих там данных, и итоговое содержимое файла оказывается `"AH"`, а не тем, что реально нужно было сохранить.

**Исправление:** отделить объём, который физически копируется в буфер (`to_copy`, обрезанный до `MAX_SIZE`), от значения, возвращаемого как отчёт о принятых байтах (`count`, исходное, не обрезанное). Модуль обрезает данные по ограниченному размеру буфера, но обязан сообщить userspace, что весь объём принят, иначе включается механизм повторной записи, который ломает уже сохранённые данные.

```c
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
```

После исправления повторная запись `"HEY, BRUDDAH"` даёт ожидаемый результат: `"HEY, BRUDD"` в 10 байт.