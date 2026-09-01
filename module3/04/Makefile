CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -g

.PHONY: all clean

all: producer consumer

producer: producer.c sem_shm_list.h
	$(CC) $(CFLAGS) -o producer producer.c

consumer: consumer.c sem_shm_list.h
	$(CC) $(CFLAGS) -o consumer consumer.c

clean:
	rm -f producer consumer
	rm -f /tmp/m3t4_key
	@ipcs -m | awk '/0x/{print $$2}' | xargs -r -n1 -I{} ipcrm -m {} 2>/dev/null || true
	@ipcs -s | awk '/0x/{print $$2}' | xargs -r -n1 -I{} ipcrm -s {} 2>/dev/null || true
