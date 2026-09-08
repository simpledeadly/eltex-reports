# Отчёт — Задание 8 (UDP RAW sockets)

## Фильтр 1: chat (порт 12345)

**Программа (Arch):**
```
sudo ./raw_sniffer chat
sniffer: capturing UDP port 12345, Ctrl+C to stop
[t=47s] MAC 12:ff:26:89:f3:69 -> ff:ff:ff:ff:ff:ff | IP 192.168.0.102:12345 -> 192.168.0.255:12345
```

**Источник трафика (macOS, chat-клиент из задания 6):**
```
./main macosuser
f
```

**Сверка — tcpdump (Arch):**
```
sudo tcpdump -i enp3s0 udp port 12345 -n
tcpdump: verbose output suppressed, use -v[v]... for full protocol decode
listening on enp3s0, link-type EN10MB (Ethernet), snapshot length 262144 bytes
16:36:31.194421 IP 192.168.0.102.12345 > 192.168.0.255.12345: UDP, length 13
```

MAC-адреса, IP-адреса и порты, зафиксированные программой и tcpdump, совпадают — broadcast-пакет чата из задания 6 захвачен корректно.

## Фильтр 2: DNS (порт 53)

**Программа (Arch):**
```
sudo ./raw_sniffer dns
sniffer: capturing UDP port 53, Ctrl+C to stop
[t=1s] MAC 8c:89:a5:57:38:ae -> 40:ed:00:dc:5b:49 | IP 192.168.0.103:43216 -> 192.168.0.1:53
[t=1s] MAC 40:ed:00:dc:5b:49 -> 8c:89:a5:57:38:ae | IP 192.168.0.1:53 -> 192.168.0.103:43216
[t=1s] MAC 8c:89:a5:57:38:ae -> 40:ed:00:dc:5b:49 | IP 192.168.0.103:53805 -> 192.168.0.1:53
[t=1s] MAC 40:ed:00:dc:5b:49 -> 8c:89:a5:57:38:ae | IP 192.168.0.1:53 -> 192.168.0.103:53805
```

**Источник трафика:**
```
nslookup hacker.news 192.168.0.1
Server:         192.168.0.1
Address:        192.168.0.1#53
Non-authoritative answer:
Name:   hacker.news
Address: 13.249.8.88
Name:   hacker.news
Address: 13.249.8.125
Name:   hacker.news
Address: 13.249.8.49
Name:   hacker.news
Address: 13.249.8.45
Name:   hacker.news
Address: 2600:9000:2171:2a00:c:bfb4:2d00:93a1
(и другие IPv6-адреса)
```

**Сверка — tshark (Arch, эквивалент Wireshark):**
```
sudo tshark -i enp3s0 -f "udp port 53" -Y "dns.qry.name contains \"hacker.news\""
Running as user "root" and group "root". This could be dangerous.
Capturing on 'enp3s0'
    1 0.000000000  192.168.0.103 → 192.168.0.1  DNS 71 Standard query 0x0fa8 A hacker.news
    2 0.063398692  192.168.0.1 → 192.168.0.103  DNS 367 Standard query response 0x0fa8 A hacker.news A 13.249.8.88 A 13.249.8.125 A 13.249.8.49 A 13.249.8.45 ...
    3 0.065065193  192.168.0.103 → 192.168.0.1  DNS 71 Standard query 0x47f7 AAAA hacker.news
    4 0.156020877  192.168.0.1 → 192.168.0.103  DNS 527 Standard query response 0x47f7 AAAA hacker.news AAAA 2600:9000:2171:2a00:c:bfb4:2d00:93a1 ...
```

## Вывод

Программа корректно перехватывает и фильтрует UDP-сегменты по двум независимым критериям (chat-порт, DNS-порт), извлекая все требуемые поля: время с начала захвата, MAC-адреса отправителя и получателя, IP-адреса отправителя и получателя, номера UDP-портов отправителя и получателя.
