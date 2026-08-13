CC = gcc
CFLAGS = -Wall -Wextra -std=c11
PLUGIN_DIR = plugins

PLUGINS = add sub mul div

PLUGIN_BINS = $(patsubst %,$(PLUGIN_DIR)/lib%.dylib,$(PLUGINS))

all: main $(PLUGIN_BINS)

$(PLUGIN_DIR)/lib%.dylib: %.c
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@

main: main.c plugin_logic.c tui.c
	$(CC) $(CFLAGS) main.c plugin_logic.c tui.c -o main

test: test.c plugin_logic.c $(PLUGIN_BINS)
	$(CC) $(CFLAGS) test.c plugin_logic.c -I/opt/homebrew/include -L/opt/homebrew/lib -lcunit -o test
	./test

clean:
	rm -f main test $(PLUGIN_DIR)/*.dylib

.PHONY: all clean test
