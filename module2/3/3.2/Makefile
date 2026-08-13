CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SOURCES = main.c subnet_logic.c
TARGET = main

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

test: test.c subnet_logic.c
	$(CC) $(CFLAGS) -o test test.c subnet_logic.c -I/opt/homebrew/include -L/opt/homebrew/lib -lcunit
	./test

clean:
	rm -f $(TARGET) test
