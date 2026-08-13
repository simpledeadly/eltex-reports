CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: main

main: main.c phonebook.c tui.c
	$(CC) $(CFLAGS) -o main main.c phonebook.c tui.c

test: test.c phonebook.c tui.c
	$(CC) $(CFLAGS) -o test test.c phonebook.c tui.c -I/opt/homebrew/include -L/opt/homebrew/lib -lcunit
	./test

clean:
	rm -f main test *.o

.PHONY: all clean test
