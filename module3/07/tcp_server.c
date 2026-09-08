#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9000
#define MAX_CLIENTS 64
#define MAX_EVENTS (MAX_CLIENTS + 1)
#define BUF_SIZE (1024 * 1024)

static int clients[MAX_CLIENTS];
static int nclients = 0;

static void client_add(int fd) {
  if (nclients < MAX_CLIENTS)
    clients[nclients++] = fd;
}

static void client_remove(int fd) {
  for (int i = 0; i < nclients; i++) {
    if (clients[i] == fd) {
      clients[i] = clients[--nclients];
      return;
    }
  }
}

static ssize_t recv_all(int fd, void *buf, size_t len) {
  size_t total = 0;
  while (total < len) {
    ssize_t r = recv(fd, (char *)buf + total, len - total, 0);
    if (r <= 0)
      return r;
    total += (size_t)r;
  }
  return (ssize_t)total;
}

static void broadcast(int sender_fd, const char *data, uint32_t len) {
  uint32_t net_len = htonl(len);
  for (int i = 0; i < nclients; i++) {
    if (clients[i] == sender_fd)
      continue;
    send(clients[i], &net_len, sizeof(net_len), 0);
    send(clients[i], data, len, 0);
  }
}

int main(void) {
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  int reuse = 1;
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    perror("setsockopt SO_REUSEADDR");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr;
  bzero((char *)&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(listen_fd, 5) < 0) {
    perror("listen");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }
  printf("server: listening on port %d\n", PORT);

  int epfd = epoll_create(1);
  if (epfd < 0) {
    perror("epoll_create");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = listen_fd;
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
    perror("epoll_ctl listen_fd");
    close(epfd);
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  struct epoll_event events[MAX_EVENTS];

  while (1) {
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
    if (nfds < 0) {
      perror("epoll_wait");
      break;
    }

    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd != listen_fd)
        continue;

      struct sockaddr_in cli_addr;
      socklen_t cli_len = sizeof(cli_addr);
      int client_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &cli_len);
      if (client_fd < 0) {
        perror("accept");
        continue;
      }

      char ip[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &cli_addr.sin_addr, ip, sizeof(ip));
      printf("server: +connect [%s:%d] fd=%d\n", ip, ntohs(cli_addr.sin_port),
             client_fd);

      ev.events = EPOLLIN;
      ev.data.fd = client_fd;
      if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
        perror("epoll_ctl client_fd");
        close(client_fd);
        continue;
      }

      client_add(client_fd);
      printf("server: clients online: %d\n", nclients);
    }

    for (int i = 0; i < nfds; i++) {
      int fd = events[i].data.fd;
      if (fd == listen_fd)
        continue;

      uint32_t net_len;
      ssize_t r = recv_all(fd, &net_len, sizeof(net_len));
      if (r <= 0) {
        printf("server: -disconnect fd=%d\n", fd);
        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
        client_remove(fd);
        close(fd);
        printf("server: clients online: %d\n", nclients);
        continue;
      }

      uint32_t body_len = ntohl(net_len);
      if (body_len == 0 || body_len > BUF_SIZE) {
        fprintf(stderr, "server: bad body_len=%u from fd=%d\n", body_len, fd);
        continue;
      }

      char *buf = malloc(body_len);
      if (!buf) {
        perror("malloc");
        continue;
      }

      r = recv_all(fd, buf, body_len);
      if (r <= 0) {
        free(buf);
        printf("server: -disconnect fd=%d\n", fd);
        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
        client_remove(fd);
        close(fd);
        printf("server: clients online: %d\n", nclients);
        continue;
      }

      if (strncmp(buf, "FILE:", 5) == 0) {
        char *nl = memchr(buf + 5, '\n', body_len - 5);
        if (nl) {
          *nl = '\0';
          printf("server: file \"%s\" from fd=%d (%u bytes)\n", buf + 5, fd,
                 body_len);
          *nl = '\n';
        }
      } else {
        printf("server: msg from fd=%d: %.*s\n", fd, (int)(body_len - 4),
               buf + 4);
      }

      broadcast(fd, buf, body_len);
      free(buf);
    }
  }

  close(epfd);
  close(listen_fd);
  return 0;
}