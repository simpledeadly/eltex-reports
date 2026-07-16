# ОТЧËТ О ВЫПОЛНЕНИИ ПРАКТИКИ 1 МОДУЛЯ 1

**Студент:** Сикаченко Дмитрий Константинович  
**Пользователь:** eltex-v34  
**Сервер:** 217.71.138.1  

---

## РАЗДЕЛ 1. Работа с мультиплексором терминалов `screen`

### Шаг 1. Подключение к серверу по протоколу SSH

Для подключения к удаленному серверу использовался стандартный SSH-клиент. Подключение выполнено на порт `44556` под учетной записью `eltex-v34`:

```bash
ssh -p 44556 eltex-v34@217.71.138.1
```

**Вывод терминала при подключении:**

```bash
eltex-v34@217.71.138.1's password: 
Linux eltex-2026-summer 6.1.0-50-amd64 #1 SMP PREEMPT_DYNAMIC Debian 6.1.176-1 (2026-07-02) x86_64

The programs included with the Debian GNU/Linux system are free software;
the exact distribution terms for each program are described in the
individual files in /usr/share/doc/*/copyright.

Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent
permitted by applicable law.
Last login: Mon Jul  6 12:32:18 2026 from 46.28.68.126
eltex-v34@eltex-2026-summer:~$ 
```

---

### Шаг 2. Запуск базовой сессии `screen` и просмотр пользователей

1. Запущена новая сессия утилиты `screen` с именем по умолчанию:

   ```bash
   screen
   ```

2. Внутри сессии выполнена команда `w` для просмотра списка активных пользователей на сервере:

   ```bash
   w
   ```

**Результат выполнения команды `w`:**

```bash
eltex-v2  pts/1    92.125.152.58    12:12    9.00s  0.01s  ?  screen
eltex-v1  pts/2    217.217.245.65   12:15    4.00s  0.00s  ?  screen -r 1700.top
eltex-v2  pts/4    92.125.152.58    12:14   28:28   0.00s  ?  -bash
eltex-v2  pts/5    92.125.152.58    12:13    6:57   0.01s  ?  screen
eltex-v4  pts/6    92.125.152.58    12:14    9.00s  0.01s  0.01s -bash
eltex-v7  pts/8    92.125.152.58    12:14    1:37   0.01s  0.01s -bash
eltex-v1  pts/11   92.125.152.58    12:17   14.00s  0.03s  0.01s screen -r top
eltex-v2  pts/13   92.125.152.58    12:16    1.00s  0.01s  0.01s -bash
eltex-v34 pts/48   92.125.152.58    12:41    1.00s  0.00s  0.01s screen
```

1. Выполнено отсоединение от сессии без завершения запущенных в ней процессов с помощью комбинации клавиш **Ctrl+a, d** (detach).

**Вывод терминала при отсоединении:**

```bash
[detached from 15043.pts-48.eltex-2026-summer]
eltex-v34@eltex-2026-summer:~$ 
```

---

### Шаг 3. Запуск отсоединенной сессии с утилитой `top`

Создана новая сессия под именем `top` в фоновом (отсоединенном) режиме, внутри которой запущена утилита мониторинга процессов `top`:

```bash
screen -dmS top top
```

---

### Шаг 4. Получение списка активных сессий

Выведен список всех созданных сессий для проверки их состояния:

```bash
screen -ls
```

**Список активных сессий `screen`:**

```bash
eltex-v34@eltex-2026-summer:~$ screen -ls
There are screens on:
 21602.top (07/06/2026 12:47:34 PM) (Detached)
 15043.pts-48.eltex-2026-summer (07/06/2026 12:41:38 PM) (Detached)
 11682.pts-57.eltex-2026-summer (07/06/2026 12:37:32 PM) (Detached)
 8363.pts-56.eltex-2026-summer (07/06/2026 12:33:11 PM) (Detached)
4 Sockets in /run/screen/S-eltex-v34.
```

---

### Шаг 5. Подключение к фоновой сессии `top`

Выполнено подключение (восстановление) к ранее созданной сессии `top`:

```bash
screen -r top
```

**Интерфейс утилиты `top` внутри сессии:**

```bash
top - 12:48:19 up  9:31, 67 users,  load average: 0.11, 0.12, 0.10
Tasks: 463 total,   1 running, 460 sleeping,   2 stopped,   0 zombie
%Cpu(s):  0.0 us,  0.0 sy,  0.0 ni,100.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
MiB Mem :   3914.7 total,   2962.0 free,    835.0 used,    338.5 buff/cache
MiB Swap:    976.0 total,    976.0 free,      0.0 used.   3079.7 avail Mem

  PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
    1 root      20   0  168740  13244   9216 S   0.0   0.3   0:01.29 systemd
    2 root      20   0       0      0      0 S   0.0   0.0   0:00.00 kthreadd
    3 root       0 -20       0      0      0 I   0.0   0.0   0:00.00 rcu_gp
    4 root       0 -20       0      0      0 I   0.0   0.0   0:00.00 rcu_par_gp
```

---

### Шаги 6–8. Создание нового окна в сессии и запуск мониторинга `vmstat`

1. Внутри активной сессии `top` создано второе окно с помощью комбинации клавиш **Ctrl+a, c**.
2. В созданном окне запущена утилита наблюдения за виртуальной памятью:

   ```bash
   watch /usr/bin/vmstat
   ```

**Вывод команды `watch /usr/bin/vmstat` в окне 1:**

```bash
Every 2.0s: /usr/bin/vmstat                                       eltex-2026-summer: Mon Jul  6 12:50:43 2026

procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 2  2      0 3012348  28632 318904    0    0     1     2   19   19  0  0 100  0  0
```

---

### Шаги 9–11. Просмотр списка окон, переименование и переключение

1. Для просмотра списка окон внутри сессии использована комбинация **Ctrl+a, "**.
2. Второе окно переименовано в `vmstat` с помощью комбинации **Ctrl+a, A** и ввода имени.
3. Выполнен переход обратно в первое окно с утилитой `top` с помощью комбинации **Ctrl+a, 0**.

**Отображение списка окон в утилите `screen` после переименования:**

```bash
Num Name                                                                                             Flags

  0 top                                                                                                 $
  1 bash                                                                                                $
  2 vmstat                                                                                              $
```

---

### Шаги 12–14. Отсоединение и создание сессии с редактором Vim

1. Выполнено отсоединение от сессии `top` с помощью комбинации **Ctrl+a, d**.
2. Запущена новая именованная сессия `vi_session` с автоматическим открытием текстового редактора `vi` для создания файла отчета:

   ```bash
   screen -S vi_session vi report_part1.txt
   ```

3. Сессия переведена в фоновый режим с помощью **Ctrl+a, d**.

**Вывод терминала при создании и отсоединении сессии редактора:**

```bash
eltex-v34@eltex-2026-summer:~$ screen -S vi_session vi report_part1.txt
[detached from 42498.vi_session]
eltex-v34@eltex-2026-summer:~$ 
```

---

### Шаги 15–18. Отключение от сервера, повторное подключение и завершение работы

1. Произведен выход из SSH-сессии с помощью комбинации клавиш **Ctrl+D**.
2. Выполнено повторное подключение к серверу по SSH.
3. Выполнено подключение к сессии с редактором (`screen -r vi_session`).
4. В файл `report_part1.txt` внесен текст отчета.

**Содержимое отредактированного файла в Vim (`:wq`):**

```text
Сикаченко Дмитрий Константинович часть 1 модуля 1 успешно завершена 06.07.2026 13:00
~
~
~
"report_part1.txt" 2L, 143B written
```

1. Все оставшиеся сессии `screen` принудительно завершены из основной консоли, проведена проверка:

**Процесс завершения сессий и проверка:**

```bash
eltex-v34@eltex-2026-summer:~$ screen -S 15043.pts-48.eltex-2026-summer -X quit
eltex-v34@eltex-2026-summer:~$ screen -S 11682.pts-57.eltex-2026-summer -X quit
eltex-v34@eltex-2026-summer:~$ screen -S 8363.pts-56.eltex-2026-summer -X quit
eltex-v34@eltex-2026-summer:~$ screen -ls
No Sockets found in /run/screen/S-eltex-v34.
```

---

## РАЗДЕЛ 2. История команд и переменные окружения

### Часть 1. Работа с историей команд и автодополнением

#### Шаг 1. Логирование сессии

Запущена новая сессия с автоматическим вычислением имени на основе текущего пользователя `eltex-v34_part2` и включено логирование:

```bash
screen -S $(whoami)_part2
exec > >(tee -a ~/eltex-v34_part.log) 2>&1
```

#### Шаг 2. Автодополнение команд на «ls»

С помощью двойного нажатия клавиши **Tab** после ввода `ls` выведен список доступных команд:

```bash
eltex-v34@eltex-2026-summer:~$ ls
ls         lsblk      lscpu      lsinitramfs lsirq      lslogins   lsmod      lsof       lsusb
lsattr     ls_release lsfd       lsipc       lslocks    lsmem      lsns       lspci
```

#### Шаг 3. Автодополнение переменных на «HIST»

Для вывода списка переменных окружения, начинающихся с `HIST`, в командной строке было набрано `echo $HIST` и дважды нажата клавиша **Tab**:

```bash
eltex-v34@eltex-2026-summer:~$ echo $HIST
$HISTCONTROL      $HISTFILE         $HISTFILESIZE     $HISTSIZE         $HISTTIMEFORMAT
```

#### Шаг 4. Размер файла истории

Для определения лимита количества хранимых команд в истории были проанализированы переменные окружения:

```bash
eltex-v34@eltex-2026-summer:~$ echo $HISTSIZE; echo $HISTFILESIZE
2000
2000
```

#### Шаг 5. Вывод скрытых файлов домашнего каталога

Выполнен вывод имен скрытых файлов и каталогов из домашней директории:

```bash
eltex-v34@eltex-2026-summer:~$ ls -d ~/.*
/home/eltex-v34/.bash_history  /home/eltex-v34/.bashrc  /home/eltex-v34/.log  /home/eltex-v34/.viminfo
/home/eltex-v34/.bash_logout   /home/eltex-v34/.config  /home/eltex-v34/.profile
```

#### Шаг 6. Настройка вывода даты и времени в истории команд

Для отображения точного времени запуска команд настроена переменная `$HISTTIMEFORMAT` и просмотрена история:

```bash
eltex-v34@eltex-2026-summer:~$ HISTTIMEFORMAT="%d/%m/%Y %T "
eltex-v34@eltex-2026-summer:~$ history | head -10
    1  06/07/2026 13:15:20 ls
    2  06/07/2026 13:15:20 pwd
    3  06/07/2026 13:15:20 ls -la ..
    4  06/07/2026 13:15:20 ls
    5  06/07/2026 13:15:20 clear
    6  06/07/2026 13:15:20 screen
    7  06/07/2026 13:15:20 clear
    8  06/07/2026 13:15:20 screen
    9  06/07/2026 13:15:20 clear
   10  06/07/2026 13:15:20 screen w
```

#### Шаг 7. Настройка моментальной записи команд в историю (`PROMPT_COMMAND`)

Проведен тест записи команд на диск до и после изменения переменной `PROMPT_COMMAND`:

```bash
eltex-v34@eltex-2026-summer:~$ date
Mon Jul  6 01:18:05 PM +07 2026
eltex-v34@eltex-2026-summer:~$ history | grep date
   67  06/07/2026 13:18:05 date
   68  06/07/2026 13:18:09 history | grep date
eltex-v34@eltex-2026-summer:~$ cat ~/.bash_history | grep date
# (Вывод пустой, так как команда еще не записана на диск)

eltex-v34@eltex-2026-summer:~$ PROMPT_COMMAND="history -a"
eltex-v34@eltex-2026-summer:~$ date
Mon Jul  6 01:18:22 PM +07 2026
eltex-v34@eltex-2026-summer:~$ cat ~/.bash_history | grep date
date
history | grep date
cat ~/.bash_history | grep date
date
```

---

### Часть 2. Переменные окружения и автоматизация

#### Шаги 1–3. Создание и объединение переменных

Созданы переменные, содержащие текущие дату и время, а также их объединенное значение:

```bash
eltex-v34@eltex-2026-summer:~$ DATE=$(date +%Y-%m-%d)
eltex-v34@eltex-2026-summer:~$ echo $DATE
2026-07-06
eltex-v34@eltex-2026-summer:~$ TIME=$(date +%H:%M:%S)
eltex-v34@eltex-2026-summer:~$ echo $TIME
13:20:30
eltex-v34@eltex-2026-summer:~$ DATE_TIME="$DATE $TIME"
eltex-v34@eltex-2026-summer:~$ echo $DATE_TIME
2026-07-06 13:20:30
```

#### Шаг 4. Поиск файлов с цифрами в названии

Осуществлен вывод файлов, содержащих в своем имени хотя бы одну цифру, из системных каталогов:

```bash
eltex-v34@eltex-2026-summer:~$ ls /bin /sbin | grep '[0-9]'
addr2line
b2sum
base32
base64
bunzip2
bzip2
bzip2recover
c89
c89-gcc
c99
c99-gcc
cpp-12
```

#### Шаг 5–6. Изменение приглашения командной строки (`PS1`)

Настроена и экспортирована системная переменная `PS1`:

```bash
eltex-v34@eltex-2026-summer:~$ PS1='\u@\h-$(date +%H:%M) > '
eltex-v34@eltex-2026-summer-13:22 > export PS1
```

#### Шаг 7. Создание каталогов для всех месяцев года

Одной командной строкой с использованием автоматической подстановки года создана структура каталогов на 12 месяцев:

```bash
eltex-v34@eltex-2026-summer-13:22 > mkdir -p $(date +%Y)-{01,02,03,04,05,06,07,08,09,10,11,12}
eltex-v34@eltex-2026-summer-13:22 > ls -d $(date +%Y)-*
2026-01  2026-02  2026-03  2026-04  2026-05  2026-06  2026-07  2026-08  2026-09  2026-10  2026-11  2026-12
```

---

## РАЗДЕЛ 3. Файловые операции, ссылки и поиск

### Шаг 1. Создание лог-файла раздела

Перешли в домашний каталог и создали лог-файл `eltex-v34_part3.log`:

```bash
eltex-v34@eltex-2026-summer-13:23 > cd ~
eltex-v34@eltex-2026-summer-13:23 > touch ${USER}_part3.log
```

*Примечание:* Все вводимые ниже команды Раздела 3 записывались в созданный лог-файл с помощью конструкции `echo "<команда>" >> ~/eltex-v34_part3.log`.

### Шаги 2–4. Создание структуры папок и файла с текстом

Создана структура вложенных каталогов, файл `file.txt` и в него записан текст:

```bash
eltex-v34@eltex-2026-summer-13:23 > mkdir -p D1/D2/D3
eltex-v34@eltex-2026-summer-13:24 > touch D1/D2/file.txt
eltex-v34@eltex-2026-summer-13:24 > echo "some text here" > D1/D2/file.txt
```

### Шаги 5–6. Создание жесткой и символической ссылок

В каталоге `D3` созданы жесткая и символическая ссылки на `file.txt`, после чего выведен список файлов:

```bash
eltex-v34@eltex-2026-summer:~$ ln -s ~/D1/D2/file.txt ~/D1/D2/D3/file_symlink.txt
eltex-v34@eltex-2026-summer:~$ ln ~/D1/D2/file.txt ~/D1/D2/D3/file_hardlink.txt
eltex-v34@eltex-2026-summer:~$ ls -la D1/D2/D3/
total 12
drwxr-xr-x 2 eltex-v34 eltex-v34 4096 Jul  6 13:26 .
drwxr-xr-x 3 eltex-v34 eltex-v34 4096 Jul  6 13:24 ..
-rw-r--r-- 2 eltex-v34 eltex-v34   15 Jul  6 13:25 file_hardlink.txt
lrwxrwxrwx 1 eltex-v34 eltex-v34   30 Jul  6 13:26 file_symlink.txt -> /home/eltex-v34/D1/D2/file.txt
```

### Шаги 7–9. Перемещение файла и проверка работы ссылок

Оригинальный файл перемещен в директорию `D1`, проведена проверка доступности данных по ссылкам:

```bash
eltex-v34@eltex-2026-summer:~$ mv D1/D2/file.txt D1/
eltex-v34@eltex-2026-summer:~$ cat D1/D2/D3/file_symlink.txt
cat: D1/D2/D3/file_symlink.txt: No such file or directory

eltex-v34@eltex-2026-summer:~$ cat D1/D2/D3/file_hardlink.txt
some text here

eltex-v34@eltex-2026-summer:~$ rm -rf D1/D2
```

*Вывод:* Символическая ссылка сломалась, так как оригинальный файл по сохраненному пути был перемещен. Жесткая ссылка осталась рабочей, так как она указывает непосредственно на файловый дескриптор (inode) на диске.

---

### Шаг 10. Поиск файлов размером более 20 МБ

Выполнен поиск файлов размером более 20 МБ в корневой директории с записью в лог. Также проведена проверка реального размера утилитой `du`:

```bash
eltex-v34@eltex-2026-summer:~$ find / -type f -size +20M 2>/dev/null > ~/${USER}_part3_gt20M.log
eltex-v34@eltex-2026-summer:~$ cat ~/${USER}_part3_gt20M.log | wc -l
32

eltex-v34@eltex-2026-summer:~$ find / -type f -size +20M 2>/dev/null | xargs du -sh 2>/dev/null
31M     /usr/bin/x86_64-linux-gnu-ld.bfd
31M     /usr/lib/gcc/x86_64-linux-gnu/12/lto-dump-12
31M     /usr/lib/gcc/x86_64-linux-gnu/12/lto1
32M     /usr/lib/gcc/x86_64-linux-gnu/12/cc1
30M     /usr/lib/x86_64-linux-gnu/libicudata.so.72.1
```

---

### Шаг 11–12. Поиск файлов, измененных за последние 24 часа

Выполнен поиск обычных файлов в домашней директории, измененных в течение последних 24 часов:

```bash
eltex-v34@eltex-2026-summer:~$ find ~ -type f -mtime -1
/home/eltex-v34/.viminfo
/home/eltex-v34/report_part1.txt
/home/eltex-v34/.log
/home/eltex-v34/eltex-v34_part.log
/home/eltex-v34/D1/file.txt
/home/eltex-v34/eltex-v34_part3.log
/home/eltex-v34/.bash_history
/home/eltex-v34/eltex-v34_part3_gt20M.log
```

---

### Шаги 13–15. Определение путей и типов файлов

Определены пути исполняемого файла `find`, а также типы системных файлов:

```bash
eltex-v34@eltex-2026-summer:~$ which find
/usr/bin/find

eltex-v34@eltex-2026-summer:~$ file $(which find)
/usr/bin/find: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=5545a6c7162d325fbe5ffabcbfe4e5521b66ce70, for GNU/Linux 3.2.0, stripped

eltex-v34@eltex-2026-summer:~$ file /boot/initrd.img*
/boot/initrd.img-6.1.0-44-amd64: ASCII cpio archive (SVR4 with no CRC)
/boot/initrd.img-6.1.0-50-amd64: ASCII cpio archive (SVR4 with no CRC)
```
