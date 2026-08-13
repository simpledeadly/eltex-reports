CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: main

lib/libphonebook.a: phonebook.o tui.o
	ar rcs lib/libphonebook.a phonebook.o tui.o

phonebook.o: lib/phonebook.c
	$(CC) $(CFLAGS) -c lib/phonebook.c -Ilib -o phonebook.o

tui.o: lib/tui.c
	$(CC) $(CFLAGS) -c lib/tui.c -Ilib -o tui.o

main: main.c lib/libphonebook.a
	$(CC) $(CFLAGS) main.c -Ilib -L./lib -lphonebook -o main

test: test.c lib/libphonebook.a
	$(CC) $(CFLAGS) test.c -Ilib -L./lib -lphonebook -I/opt/homebrew/include -L/opt/homebrew/lib -lcunit -o test
	./test

clean:
	rm -f main test *.o *.a lib/*.a

.PHONY: all clean test
