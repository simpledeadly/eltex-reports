all: build run

build: main.c tui.c phonebook.c
	gcc -Wall main.c tui.c phonebook.c -o main

run: build
	./main