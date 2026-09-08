CC     = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -g

OS := $(shell uname)

ifeq ($(OS), Linux)
all: tcp_server tcp_client
else
# macOS не поддерживает epoll — собираем только клиента
all: tcp_client
	@echo "NOTE: tcp_server uses epoll (Linux only). Build on Linux."
endif

tcp_server: tcp_server.c
	$(CC) $(CFLAGS) -o tcp_server tcp_server.c

tcp_client: tcp_client.c
	$(CC) $(CFLAGS) -o tcp_client tcp_client.c

clean:
	rm -f tcp_server tcp_client
	rm -rf tcp_server.dSYM tcp_client.dSYM