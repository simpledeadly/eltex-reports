CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SOURCES = main.c
TARGET = main

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
