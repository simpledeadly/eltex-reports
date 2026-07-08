# module 1 practice 2

---

## p1

```bash
ssh -J eltex-v34@217.71.138.1:44556 root@172.16.9.208
```

step 1

```bash
eltex-v34@eltex-2026-summer:~$ ssh-keygen
Generating public/private rsa key pair.
Enter file in which to save the key (/home/eltex-v34/.ssh/id_rsa):
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /home/eltex-v34/.ssh/id_rsa
Your public key has been saved in /home/eltex-v34/.ssh/id_rsa.pub
The key fingerprint is:
SHA256:SpfSZ5fZF+RnoTgrZjBZZ+/ZNnFkAqHTwAtNqwJD7OE eltex-v34@eltex-2026-summer
The key's randomart image is:
+---[RSA 3072]----+
|   ..    =oooo.oo|
|   .o   + ==o ++.|
|   oo. + .++.o ++|
|    Eo .oo..++o.=|
|      + S+o.+o.+.|
|     . =oo..  ...|
|      .          |
|                 |
|                 |
+----[SHA256]-----+
```

step 2

```bash
eltex-v34@eltex-2026-summer:~$ ssh-copy-id root@172.16.9.208
/usr/bin/ssh-copy-id: INFO: Source of key(s) to be installed: "/home/eltex-v34/.ssh/id_rsa.pub"
/usr/bin/ssh-copy-id: INFO: attempting to log in with the new key(s), to filter out any that are already installed
/usr/bin/ssh-copy-id: INFO: 1 key(s) remain to be installed -- if you are prompted now it is to install the new keys
root@172.16.9.208's password:

Number of key(s) added: 1

Now try logging into the machine, with:   "ssh 'root@172.16.9.208'"
and check to make sure that only the key(s) you wanted were added.
```

step 3

```bash
eltex-v34@eltex-2026-summer:~$ ssh root@172.16.9.208
Welcome to Ubuntu 24.04.2 LTS (GNU/Linux 6.8.0-55-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/pro

 System information as of Wed Jul  8 04:58:08 AM UTC 2026

  System load:  0.01               Processes:              112
  Usage of /:   37.8% of 14.66GB   Users logged in:        0
  Memory usage: 5%                 IPv4 address for ens18: 172.16.9.208
  Swap usage:   0%


Expanded Security Maintenance for Applications is not enabled.

0 updates can be applied immediately.

Enable ESM Apps to receive additional future security updates.
See https://ubuntu.com/esm or run: sudo pro status


The list of available updates is more than a week old.
To check for new updates run: sudo apt update

Last login: Wed Jul  8 04:56:58 2026 from 172.16.8.4
root@eltex-practice2-v34:~#
```

```bash
root@eltex-practice2-v34:~# cat .ssh/authorized_keys
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQDEGBmrAthpBE9AMrRMYxDXqzCB4alkIUM4fDAqaUEiXvQFFp0/g8kRajFpUVG4HyTIOyVpLDGf2ugO0DwOO1jgMKN2Dc1JH2j33cf+wc1P3KFxhOX9bbhukSlAsaUE7tUkBz2M2jiaDJ1bYzzWqFxkrlbQErAbR6n4Dl10CdCDmzoceGji3h9t/iYffJy4SQVWvfYsDzDHX/7P0UXGc2jTPmgB3bcHqVTdFiicKXNGGuZoZncZZHH9mAV97PB+QCjX/tO1tCQGR5W3eCoAJsmB+A3LQT3NDZ1bJvACSAjeurExSSpZQxeUT4UDG2FxEc3DCJrUIM8PdlDnggGwvIyBG9hfNJKfqLhY1+/EUmLSUabEU4IHxEK8OrPLnCIqrLyKQTQpJMmDlvo8Gx9tXWuJDhl8Qq17SrRnBZp4CKApoI8SYorjnCDQ6V32wDEXab1ZgCquTMFYI/1el0crPCrDjz5ubBz2bW/QhE8lGz4uPYxDf1ctidgdYADw7lkzopU= eltex-v34@eltex-2026-summer
root@eltex-practice2-v34:~# ls -l .ssh/authorized_keys
-rw------- 1 root root 581 Jul  8 04:54 .ssh/authorized_keys
```

logging

```bash
MYUSER=eltex-v34
script -a practice2_${MYUSER}_part1.log
```

added user and set password

```bash
root@eltex-practice2-v34:~# useradd -m -s /bin/bash user1
useradd: user 'user1' already exists
root@eltex-practice2-v34:~# passwd user1
New password:
Retype new password:
passwd: password updated successfully
```

added user2

```bash
root@eltex-practice2-v34:~# adduser2
Command 'adduser2' not found, did you mean:
  command 'adduser' from deb adduser (3.137ubuntu1)
Try: apt install <deb name>
root@eltex-practice2-v34:~# adduser user2
info: Adding user `user2' ...
info: Selecting UID/GID from range 1000 to 59999 ...
info: Adding new group `user2' (1002) ...

info: Adding new user `user2' (1002) with group `user2 (1002)' ...
info: Creating home directory `/home/user2' ...
info: Copying files from `/etc/skel' ...
New password:
Retype new password:
passwd: password updated successfully
Changing the user information for user2
Enter the new value, or press ENTER for the default
 Full Name []: Usertwo Twoich Userovich
 Room Number []: 2
 Work Phone []: 222
 Home Phone []: 222
 Other []: nothing, just second user!
Is the information correct? [Y/n] y
info: Adding new user `user2' to supplemental / extra groups `users' ...
info: Adding user `user2' to group `users' ...
```

added user3

```bash
root@eltex-practice2-v34:~# adduser user3
info: Adding user `user3' ...
info: Selecting UID/GID from range 1000 to 59999 ...
info: Adding new group `user3' (1003) ...
info: Adding new user `user3' (1003) with group `user3 (1003)' ...
info: Creating home directory `/home/user3' ...
info: Copying files from `/etc/skel' ...
New password:
Retype new password:
passwd: password updated successfully
Changing the user information for user3
Enter the new value, or press ENTER for the default
 Full Name []: Userthree Thirdich Usertwoevich
 Room Number []: 3
 Work Phone []: 333
 Home Phone []: 333
 Other []: nothing again, it's just a third user!!!
Is the information correct? [Y/n] y
info: Adding new user `user3' to supplemental / extra groups `users' ...
info: Adding user `user3' to group `users' ...
```

user3 sueta

```bash
root@eltex-practice2-v34:~# usermod -s /usr/sbin/nologin user3
root@eltex-practice2-v34:~# su user3
This account is currently not available.
root@eltex-practice2-v34:~# su -s /bin/bash user3
user3@eltex-practice2-v34:/root$ logout
bash: logout: not login shell: use `exit'
user3@eltex-practice2-v34:/root$ echo "i knew it!"
i knew it!
user3@eltex-practice2-v34:/root$ exit
exit
```

created group and added users

```bash
root@eltex-practice2-v34:~# groupadd sharedgroup
root@eltex-practice2-v34:~# usermod -aG sharedgroup user1
root@eltex-practice2-v34:~# usermod -aG sharedgroup user2
root@eltex-practice2-v34:~# usermod -aG sharedgroup user3
root@eltex-practice2-v34:~# groups user1 user2 user3
user1 : user1 sharedgroup
user2 : user2 users sharedgroup
user3 : user3 users sharedgroup
```

created dir and chmod

```bash
root@eltex-practice2-v34:~# mkdir -p /opt/share
root@eltex-practice2-v34:~# chgrp sharedgroup /opt/share
root@eltex-practice2-v34:~# chmod g+rwx,r+s /opt/share
chmod: invalid mode: ‘g+rwx,r+s’
Try 'chmod --help' for more information.
root@eltex-practice2-v34:~# chmod g+rwx,g+s /opt/share
root@eltex-practice2-v34:~# echo "umask 0027" >> /home/user1/.bashrc
```

created files by each user and deleting tries

```bash
root@eltex-practice2-v34:~# su - user1 -c "touch /opt/share/file1.txt"
root@eltex-practice2-v34:~# su - user2 -c "touch /opt/share/file2.txt"
root@eltex-practice2-v34:~# su - user3 -s /bin/bash -c "touch /opt/share/file3.txt"
root@eltex-practice2-v34:~# ls -lA /opt/share
total 0
-rw-rw-r-- 1 user1 sharedgroup 0 Jul  8 05:27 file1.txt
-rw-rw-r-- 1 user2 sharedgroup 0 Jul  8 05:27 file2.txt
-rw-rw-r-- 1 user3 sharedgroup 0 Jul  8 05:28 file3.txt
root@eltex-practice2-v34:~# su - user1 -c "rm /opt/share/file2.txt"
root@eltex-practice2-v34:~# ls -lA /opt/share
total 0
-rw-rw-r-- 1 user1 sharedgroup 0 Jul  8 05:27 file1.txt
-rw-rw-r-- 1 user3 sharedgroup 0 Jul  8 05:28 file3.txt
root@eltex-practice2-v34:~# chmod +t /opt/share
root@eltex-practice2-v34:~# su - user2 -c "touch /opt/share/file2_new.txt"
root@eltex-practice2-v34:~# su - user1 -c "rm /opt/share/file2.txt"
rm: cannot remove '/opt/share/file2.txt': No such file or directory
root@eltex-practice2-v34:~# su - user1 -c "rm /opt/share/file2_new.txt"
rm: cannot remove '/opt/share/file2_new.txt': Operation not permitted
root@eltex-practice2-v34:~# ls -lA /opt/share
total 0
-rw-rw-r-- 1 user1 sharedgroup 0 Jul  8 05:27 file1.txt
-rw-rw-r-- 1 user2 sharedgroup 0 Jul  8 05:28 file2_new.txt
-rw-rw-r-- 1 user3 sharedgroup 0 Jul  8 05:28 file3.txt
```

sudo dmesg for user1

```bash
root@eltex-practice2-v34:~# su - user1 -c "sudo dmesg | tail -n 5"
[   55.481900] audit: type=1400 audit(1783471833.117:121): apparmor="STATUS" operation="profile_replace" info="same as current profile, skipping" profile="unconfined" name="rsyslogd" pid=722 comm="apparmor_parser"
[   91.013294] loop0: detected capacity change from 0 to 8
[   91.140813] audit: type=1400 audit(1783471868.345:122): apparmor="STATUS" operation="profile_replace" profile="unconfined" name="/usr/lib/snapd/snap-confine" pid=838 comm="apparmor_parser"
[   91.146691] audit: type=1400 audit(1783471868.351:123): apparmor="STATUS" operation="profile_replace" profile="unconfined" name="/usr/lib/snapd/snap-confine//mount-namespace-capture-helper" pid=838 comm="apparmor_parser"
[   93.024277] NET: Registered PF_QIPCRTR protocol family
```

user2 sueta

```bash
root@eltex-practice2-v34:~# echo '#!/bin/bash' > /usr/local/bin/run_dmesg
root@eltex-practice2-v34:~# echo '/usr/bin/dmesg' >> /usr/local/bin/run_dmesg
root@eltex-practice2-v34:~# chown root:root /usr/local/bin/run_dmesg
root@eltex-practice2-v34:~# chmod u+s,a+x /usr/local/bin/run_dmesg
root@eltex-practice2-v34:~# su - user2 -c "/usr/local/bin/run_dmesg"
dmesg: read kernel buffer failed: Operation not permitted
root@eltex-practice2-v34:~# ls -l /usr/local/bin/run_dmesg
-rwsr-xr-x 1 root root 27 Jul  8 05:35 /usr/local/bin/run_dmesg
```

now password for user* will be actual 10 days, then added my name to certain file

```bash
root@eltex-practice2-v34:~# chage -M 10 user1; chage -M 10 user2; chage -M 10 user3

root@eltex-practice2-v34:~#
root@eltex-practice2-v34:~# echo "Sikachenko Dmitriy" > /etc/motd
```

```bash
root@eltex-practice2-v34:~# rsync -a /etc/ /root/etc_backup/
root@eltex-practice2-v34:~# tar -czf etc_backup.tar.gz /root/etc_backup/
tar: Removing leading `/' from member name
root@eltex-practice2-v34:~# tar -cjf etc_backup.tar.bz2 /root/etc_backup
tar: Removing leading `/' from member names
```

install 7zip, like bzip2 earlier

```bash
root@eltex-practice2-v34:~# apt install p7zip-full -y
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following additional packages will be installed:
  7zip
Suggested packages:
  7zip-standalone 7zip-rar
The following NEW packages will be installed:
  7zip p7zip-full
0 upgraded, 2 newly installed, 0 to remove and 5 not upgraded.
Need to get 1,849 kB of archives.
After this operation, 6,318 kB of additional disk space will be used.
Get:1 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 7zip amd64 23.01+dfsg-11 [1,846 kB]
Get:2 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 p7zip-full all 16.02+transitional.1 [2,500 B]
Fetched 1,849 kB in 1s (3,259 kB/s)
Selecting previously unselected package 7zip.
(Reading database ... 86697 files and directories currently installed.)
Preparing to unpack .../7zip_23.01+dfsg-11_amd64.deb ...
Unpacking 7zip (23.01+dfsg-11) ...
Selecting previously unselected package p7zip-full.
Preparing to unpack .../p7zip-full_16.02+transitional.1_all.deb ...
Unpacking p7zip-full (16.02+transitional.1) ...
Setting up 7zip (23.01+dfsg-11) ...
Setting up p7zip-full (16.02+transitional.1) ...
Processing triggers for man-db (2.12.0-4build2) ...
Scanning processes...
Scanning linux images...

Running kernel seems to be up-to-date.

No services need to be restarted.

No containers need to be restarted.

No user sessions are running outdated binaries.

No VM guests are running outdated hypervisor (qemu) binaries on this host.
root@eltex-practice2-v34:~#

```

7zip and list of zips, 7zip the biggest in the list

```bash
root@eltex-practice2-v34:~# 7z a etc_backup.7z /root/etc_backup

7-Zip 23.01 (x64) : Copyright (c) 1999-2023 Igor Pavlov : 2023-06-20
 64-bit locale=en_US.UTF-8 Threads:2 OPEN_MAX:1024

Scanning the drive:

WARNING: errno=2 : No such file or directory
/root/etc_backup/os-release


WARNING: errno=2 : No such file or directory
/root/etc_backup/resolv.conf


WARNING: errno=2 : No such file or directory
/root/etc_backup/mtab

244 folders, 1507 files, 38698366 bytes (37 MiB)

Creating archive: etc_backup.7z

Add new data to archive: 244 folders, 1507 files, 38698366 bytes (37 MiB)


Files read from disk: 1494
Archive size: 4676518 bytes (4567 KiB)

Scan WARNINGS for files and folders:

/root/etc_backup/os-release : errno=2 : No such file or directory
/root/etc_backup/resolv.conf : errno=2 : No such file or directory
/root/etc_backup/mtab : errno=2 : No such file or directory
----------------
Scan WARNINGS: 3
root@eltex-practice2-v34:~# ls -lh  etc_backup.*
-rw-r--r-- 1 root root 4.5M Jul  8 05:49 etc_backup.7z
-rw-r--r-- 1 root root 554K Jul  8 05:47 etc_backup.tar.bz2
-rw-r--r-- 1 root root 582K Jul  8 05:42 etc_backup.tar.gz
```

date to file, synchronization and motd to compressed zip

```bash
root@eltex-practice2-v34:~# date > /etc/motd
root@eltex-practice2-v34:~# date
Wed Jul  8 05:51:44 AM UTC 2026
root@eltex-practice2-v34:~# rsync -a /etc/ /root/etc_backup/
root@eltex-practice2-v34:~# gunzip etc_backup.tar.gz
root@eltex-practice2-v34:~# tar -rf etc_backup.tar /etc/motd
tar: Removing leading `/' from member names
tar: Removing leading `/' from hard link targets
root@eltex-practice2-v34:~# gzip etc_backup.tar
```

comparison bzip to 7zip

```bash
root@eltex-practice2-v34:~# tar -df etc_backup.tar.bz2 -C /
root/etc_backup/motd: Mod time differs
root/etc_backup/motd: Size differs
root@eltex-practice2-v34:~# mkdir -p /root/etc_backup_gzip /root/etc_backup_7zip
root@eltex-practice2-v34:~# tar -xzf etc_backup.tar.gz -C /root/etc_backup_gzip
root@eltex-practice2-v34:~# 7z x etc_backup.7z -o/root/etc_backup_7zip

7-Zip 23.01 (x64) : Copyright (c) 1999-2023 Igor Pavlov : 2023-06-20
 64-bit locale=en_US.UTF-8 Threads:2 OPEN_MAX:1024

Scanning the drive for archives:
1 file, 4676518 bytes (4567 KiB)

Extracting archive: etc_backup.7z
--
Path = etc_backup.7z
Type = 7z
Physical Size = 4676518
Headers Size = 20692
Method = LZMA2:24 BCJ
Solid = +
Blocks = 2

Everything is Ok

Folders: 244
Files: 1507
Size:       38698366
Compressed: 4676518
root@eltex-practice2-v34:~# diff /root/etc_backup_gzip/etc/motd /root/etc_backup_7zip/root/etc_backup/motd
diff: /root/etc_backup_7zip/root/etc_backup/motd: No such file or directory
root@eltex-practice2-v34:~# diff /root/etc_backup_gzip/etc/motd /root/etc_backup_7zip/root/etc_backup/motd^C
root@eltex-practice2-v34:~# ^C
root@eltex-practice2-v34:~# find /root/etc_backup_7zip -name "motd"
/root/etc_backup_7zip/etc_backup/motd
root@eltex-practice2-v34:~# diff /root/etc_backup_gzip/etc/motd /root/etc_backup_7zip/etc_backup/motd
1c1
< Wed Jul  8 05:51:42 AM UTC 2026
---
> Sikachenko Dmitriy
```

---

## p2

journalctl

```bash
root@eltex-practice2-v34:~# script -a practice2_${MYUSER}_part2.log
Script started, output log file is 'practice2_eltex-v34_part2.log'.
root@eltex-practice2-v34:~# journalctl -b --until "$(date -d "$(uptime -s) + 20 seconds" + '%Y-%m-$d %H:%M:%S')"
date: extra operand ‘%Y-%m-$d %H:%M:%S’
Try 'date --help' for more information.
Failed to parse timestamp:
root@eltex-practice2-v34:~# journalctl -b --until "$(date -d "$(uptime -s) + 20 seconds" +'%Y-%m-$d %H:%M:%S')"
Failed to parse timestamp: 2026-07-$d 04:49:38
root@eltex-practice2-v34:~# journalctl -b --until "$(date -d "$(uptime -s) + 20 seconds" +'%Y-%m-%d %H:%M:%S')"
-- No entries --
```

found auth.log file

```bash
root@eltex-practice2-v34:~# find /var/log -name auth.log
/var/log/auth.log
```

auth.log via awk

```bash
root@eltex-practice2-v34:~# awk '{print $5}' /var/log/auth.log | sed 's/\[[0-9]*\]://' | sort | uniq -c | sort -nr | head -n 20
    153 session
     20 rules
     15 password
     12 'tech'
     10 system
      8 loading,
      6 from
      6 disconnect
      6 added
      5 user1)
      5 group:
      4 user:
      4 the
      4 seat
      4 garbage
      3 user3)
      3 user2)
      3 user
      3 is
      2 'user3'
```

users with nologin via awk

```bash
root@eltex-practice2-v34:~# awk -F: '$7=="/usr/sbin/nologin" {print $3","$1"}' /etc/passwd | sort -t, -k1,1nr | while IFS=, read uid user; do groups=$(id -Gn $user | tr ' ' ','); echo "$uid, $user, $groups"; done
awk: cmd. line:1: $7=="/usr/sbin/nologin" {print $3","$1"}
awk: cmd. line:1:                                       ^ unterminated string
root@eltex-practice2-v34:~# awk -F: '$7=="/usr/sbin/nologin" {print $3","$1}' /etc/passwd | sort -t, -k1,1nr | while IFS=, read uid user; do groups=$(id -Gn $user | tr ' ' ','); echo "$uid, $user, $groups"; done
65534, nobody, nogroup
1003, user3, user3,users,sharedgroup
998, systemd-network, systemd-network
997, systemd-timesync, systemd-timesync
992, systemd-resolve, systemd-resolve
991, polkitd, polkitd
989, fwupd-refresh, fwupd-refresh
109, sshd, nogroup
108, usbmux, plugdev
107, landscape, landscape
105, tcpdump, tcpdump
104, uuidd, uuidd
103, syslog, syslog,adm
101, messagebus, messagebus
42, _apt, nogroup
39, irc, irc
38, list, list
34, backup, backup
33, www-data, www-data
13, proxy, proxy
10, uucp, uucp
9, news, news
8, mail, mail
7, lp, lp
6, man, man
5, games, games
3, sys, sys
2, bin, bin
1, daemon, daemon
```

strokes 'kernel' and their quantity

```bash
root@eltex-practice2-v34:~# dmesg | grep 'kernel'
[    0.133436] Booting paravirtualized kernel on KVM
[    0.134944] Unknown kernel command line parameters "BOOT_IMAGE=/vmlinuz-6.8.0-55-generic", will be passed to user space.
[    0.287775] Memory: 3933488K/4193760K available (22528K kernel code, 4443K rwdata, 14344K rodata, 4988K init, 4716K bss, 260012K reserved, 0K cma-reserved)
[    0.926416] Loaded X.509 cert 'Build time autogenerated kernel key: e0bedf6fa049a1e09bd65ebab3904f14cdd5bc68'
[    1.162849] Loaded X.509 cert 'Build time autogenerated kernel key: e0bedf6fa049a1e09bd65ebab3904f14cdd5bc68'
[    1.211875] Freeing unused kernel image (initmem) memory: 4988K
[    1.212888] Write protecting the kernel read-only data: 38912k
[    1.217094] Freeing unused kernel image (rodata/data gap) memory: 2040K
[   15.840586] systemd[1]: Listening on systemd-udevd-kernel.socket - udev Kernel Socket.
[   16.045388] systemd[1]: Mounting sys-kernel-debug.mount - Kernel Debug File System...
[   16.051310] systemd[1]: Mounting sys-kernel-tracing.mount - Kernel Trace File System...
[   16.559241] systemd[1]: Mounted sys-kernel-debug.mount - Kernel Debug File System.
[   16.565661] systemd[1]: Mounted sys-kernel-tracing.mount - Kernel Trace File System.
[   16.598751] systemd[1]: Mounting sys-kernel-config.mount - Kernel Configuration File System...
[   16.640000] systemd[1]: Mounted sys-kernel-config.mount - Kernel Configuration File System.
root@eltex-practice2-v34:~# wc -l /var/log/kern.log
2851 /var/log/kern.log
```

history.log formatting

```bash
root@eltex-practice2-v34:~# awk -v RS="\n\n" -v ORS="\n" '/Commandline:/ {cmd=""; start=""; end=""; split($0, a, "\n"); for(i in a){ if(a[i]~/^Commandline:/) cmd=a[i]; if(a[i]~/^Start-Date:/) start=a[i]; if(a[i]~/^End-Date:/) end=a[i]; } print cmd " ; " start " ; " end}' /var/log/apt/history.log | head -n 20
Commandline: apt-get install --reinstall apt ; Start-Date: 2025-02-16  20:51:22 ; End-Date: 2025-02-16  20:51:24
Commandline: apt-get install --reinstall base-files ; Start-Date: 2025-02-16  20:51:26 ; End-Date: 2025-02-16  20:51:28
Commandline: apt-get install --reinstall base-passwd ; Start-Date: 2025-02-16  20:51:31 ; End-Date: 2025-02-16  20:51:32
Commandline: apt-get install --reinstall bash ; Start-Date: 2025-02-16  20:51:35 ; End-Date: 2025-02-16  20:51:37
Commandline: apt-get install --reinstall bsdutils ; Start-Date: 2025-02-16  20:51:39 ; End-Date: 2025-02-16  20:51:41
Commandline: apt-get install --reinstall coreutils ; Start-Date: 2025-02-16  20:51:44 ; End-Date: 2025-02-16  20:51:45
Commandline: apt-get install --reinstall dash ; Start-Date: 2025-02-16  20:51:48 ; End-Date: 2025-02-16  20:51:49
Commandline: apt-get install --reinstall debconf ; Start-Date: 2025-02-16  20:51:52 ; End-Date: 2025-02-16  20:51:53
Commandline: apt-get install --reinstall debianutils ; Start-Date: 2025-02-16  20:51:56 ; End-Date: 2025-02-16  20:51:58
Commandline: apt-get install --reinstall diffutils ; Start-Date: 2025-02-16  20:52:00 ; End-Date: 2025-02-16  20:52:02
Commandline: apt-get install --reinstall dpkg ; Start-Date: 2025-02-16  20:52:04 ; End-Date: 2025-02-16  20:52:06
Commandline: apt-get install --reinstall e2fsprogs ; Start-Date: 2025-02-16  20:52:09 ; End-Date: 2025-02-16  20:52:10
Commandline: apt-get install --reinstall findutils ; Start-Date: 2025-02-16  20:52:13 ; End-Date: 2025-02-16  20:52:14
Commandline: apt-get install --reinstall gcc-14-base:amd64 ; Start-Date: 2025-02-16  20:52:17 ; End-Date: 2025-02-16  20:52:18
Commandline: apt-get install --reinstall gpgv ; Start-Date: 2025-02-16  20:52:21 ; End-Date: 2025-02-16  20:52:23
Commandline: apt-get install --reinstall grep ; Start-Date: 2025-02-16  20:52:25 ; End-Date: 2025-02-16  20:52:27
Commandline: apt-get install --reinstall gzip ; Start-Date: 2025-02-16  20:52:29 ; End-Date: 2025-02-16  20:52:31
Commandline: apt-get install --reinstall hostname ; Start-Date: 2025-02-16  20:52:34 ; End-Date: 2025-02-16  20:52:35
Commandline: apt-get install --reinstall init-system-helpers ; Start-Date: 2025-02-16  20:52:38 ; End-Date: 2025-02-16  20:52:39
Commandline: apt-get install --reinstall libacl1:amd64 ; Start-Date: 2025-02-16  20:52:42 ; End-Date: 2025-02-16  20:52:43
```

send third log file from mac to server 1

```bash
~/Documents/Eltex/m1p2 main 8s ❯ scp -P 44556 practice2_eltex-v34_part3.log eltex-v34@217.71.138.1:~/  13:19:49

eltex-v34@217.71.138.1's password:
practice2_eltex-v34_part3.log
```

then from server 1 to server 1

```bash
eltex-v34@eltex-2026-summer:~$ scp practice2_eltex-v34_part3.log root@172.16.9.208:~/
practice2_eltex-v34_part3.log                                                 100%   41KB  34.7MB/s   00:00
eltex-v34@eltex-2026-summer:~$ ssh root@172.16.9.208
Welcome to Ubuntu 24.04.2 LTS (GNU/Linux 6.8.0-55-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/pro

 System information as of Wed Jul  8 06:20:46 AM UTC 2026

  System load:  0.08               Processes:              112
  Usage of /:   38.4% of 14.66GB   Users logged in:        0
  Memory usage: 6%                 IPv4 address for ens18: 172.16.9.208
  Swap usage:   0%


Expanded Security Maintenance for Applications is not enabled.

0 updates can be applied immediately.

Enable ESM Apps to receive additional future security updates.
See https://ubuntu.com/esm or run: sudo pro status


Wed Jul  8 05:51:42 AM UTC 2026
Last login: Wed Jul  8 05:07:52 2026 from 172.16.8.4
root@eltex-practice2-v34:~# ls
1           etc_backup.7z    etc_backup_gzip     etc_backup.tar.gz              practice2_eltex-v34_part2.log
etc_backup  etc_backup_7zip  etc_backup.tar.bz2  practice2_eltex-v34_part1.log  practice2_eltex-v34_part3.log
```

---

_PS: After a whole journey, I thought I had captured everything — but I was wrong..._
