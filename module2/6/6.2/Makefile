CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: main

lib/libphonebook.dylib: phonebook.o tui.o
	$(CC) -shared -o lib/libphonebook.dylib phonebook.o tui.o

phonebook.o: lib/phonebook.c
	$(CC) $(CFLAGS) -fPIC -c lib/phonebook.c -Ilib -o phonebook.o

tui.o: lib/tui.c
	$(CC) $(CFLAGS) -fPIC -c lib/tui.c -Ilib -o tui.o

main: main.c lib/libphonebook.dylib
	$(CC) $(CFLAGS) main.c -Ilib -L./lib -lphonebook -Wl,-rpath,./lib -o main

test: test.c lib/libphonebook.dylib
	$(CC) $(CFLAGS) test.c -Ilib -L./lib -lphonebook -Wl,-rpath,./lib -I/opt/homebrew/include -L/opt/homebrew/lib -lcunit -o test
	./test

clean:
	rm -f main test *.o *.a lib/*.a lib/*.dylib

.PHONY: all clean test
