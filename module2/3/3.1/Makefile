CC = gcc
CFLAGS = -Wall -Wextra
SOURCES = main.c tui.c chmod_logic.c
TARGET = main

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

test: test.c chmod_logic.c
	$(CC) $(CFLAGS) -o test test.c chmod_logic.c -I/opt/homebrew/include -L/opt/homebrew/lib -lcunit
	./test

clean:
	rm -f $(TARGET) test
