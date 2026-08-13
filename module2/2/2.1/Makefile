all: build run

build: main.c tui.c phonebook.c
	gcc -Wall main.c tui.c phonebook.c -o main

test: test.c phonebook.c tui.c
	gcc -Wall test.c phonebook.c tui.c -o test -I/opt/homebrew/include -L/opt/homebrew/lib -lcunit
	./test

run: build
	./main