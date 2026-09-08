CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -g

all: raw_sniffer

raw_sniffer: raw_sniffer.c
	$(CC) $(CFLAGS) -o raw_sniffer raw_sniffer.c

clean:
	rm -f raw_sniffer