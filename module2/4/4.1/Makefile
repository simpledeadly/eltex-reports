CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: main

main: main.c phonebook.c tui.c
	$(CC) $(CFLAGS) -o main main.c phonebook.c tui.c

clean:
	rm -f main *.o

.PHONY: all clean
