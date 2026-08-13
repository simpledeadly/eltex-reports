all: build run

build: main.c tui.c calc.c
	gcc -Wall main.c tui.c calc.c -o main

test: test.c calc.c
	gcc -Wall test.c calc.c -o test -I/opt/homebrew/include -L/opt/homebrew/lib -lcunit
	./test

run: build
	./main