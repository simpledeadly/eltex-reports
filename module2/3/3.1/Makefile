CC = gcc
CFLAGS = -Wall -Wextra
SOURCES = main.c tui.c
TARGET = main

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
