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
	rm -f /dev/shm/m3t5_shm
	rm -f /dev/shm/sem.m3t5_sem
