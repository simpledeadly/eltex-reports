all: build run

build: main.c tui.c
	gcc -Wall main.c tui.c -o main

run: build
	./main