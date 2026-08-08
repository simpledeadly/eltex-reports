CC = gcc
CFLAGS = -Wall -Wextra -std=c11
PLUGIN_DIR = plugins

PLUGINS = add sub mul div

PLUGIN_BINS = $(patsubst %,$(PLUGIN_DIR)/lib%.dylib,$(PLUGINS))

all: main $(PLUGIN_BINS)

$(PLUGIN_DIR)/lib%.dylib: %.c
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@

main: main.c tui.c
	$(CC) $(CFLAGS) main.c tui.c -o main

clean:
	rm -f main $(PLUGIN_DIR)/*.dylib

.PHONY: all clean
