CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: main

main: main.c pqueue.c
	$(CC) $(CFLAGS) -o main main.c pqueue.c

test: test.c pqueue.c
	$(CC) $(CFLAGS) -o test test.c pqueue.c -I/opt/homebrew/include -L/opt/homebrew/lib -lcunit
	./test

clean:
	rm -f main test *.o

.PHONY: all clean test
