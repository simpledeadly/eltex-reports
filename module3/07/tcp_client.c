#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define PORT 9000
#define BUF_SIZE (1024 * 1024)

static char username[64];

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

static int send_packet(int sock, const char *body, uint32_t len) {
  uint32_t net_len = htonl(len);
  if (send(sock, &net_len, sizeof(net_len), 0) < 0) {
    perror("send header");
    return -1;
  }
  if (send(sock, body, len, 0) < 0) {
    perror("send body");
    return -1;
  }
  return 0;
}

static int handle_send(int sock, char *line) {
  line[strcspn(line, "\n")] = '\0';

  int is_path = (line[0] == '/') || (line[0] == '.' && line[1] == '/') ||
                (line[0] == '.' && line[1] == '.' && line[2] == '/');

  if (is_path) {
    const char *path = line;

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
      perror("open");
      return 0;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
      perror("fstat");
      close(fd);
      return 0;
    }

    const char *name = strrchr(path, '/');
    name = name ? name + 1 : path;

    size_t prefix_len = 5 + strlen(name) + 1;
    size_t total = prefix_len + (size_t)st.st_size;

    if (total > BUF_SIZE) {
      fprintf(stderr, "file too large (%zu bytes)\n", total);
      close(fd);
      return 0;
    }

    char *buf = malloc(total);
    if (!buf) {
      perror("malloc");
      close(fd);
      return 0;
    }

    int hdr_len = snprintf(buf, prefix_len + 1, "FILE:%s\n", name);
    ssize_t r = read(fd, buf + hdr_len, (size_t)st.st_size);
    close(fd);

    if (r < 0) {
      perror("read");
      free(buf);
      return 0;
    }

    printf("[file] sending \"%s\" (%zd bytes)\n", name, r);
    send_packet(sock, buf, (uint32_t)(hdr_len + r));
    free(buf);

  } else {
    char body[BUF_SIZE];
    int len = snprintf(body, sizeof(body), "MSG:%s: %s", username, line);
    if (len > 0)
      send_packet(sock, body, (uint32_t)len);
  }

  return 0;
}

static int handle_receive(int sock) {
  uint32_t net_len;
  ssize_t r = recv_all(sock, &net_len, sizeof(net_len));
  if (r <= 0)
    return -1;

  uint32_t body_len = ntohl(net_len);
  if (body_len == 0 || body_len > BUF_SIZE) {
    fprintf(stderr, "bad packet length: %u\n", body_len);
    return 0;
  }

  char *buf = malloc(body_len + 1);
  if (!buf) {
    perror("malloc");
    return 0;
  }

  r = recv_all(sock, buf, body_len);
  if (r <= 0) {
    free(buf);
    return -1;
  }
  buf[body_len] = '\0';

  if (strncmp(buf, "MSG:", 4) == 0) {
    printf("%s\n", buf + 4);
    fflush(stdout);
  } else if (strncmp(buf, "FILE:", 5) == 0) {
    char *nl = memchr(buf + 5, '\n', body_len - 5);
    if (!nl) {
      fprintf(stderr, "malformed FILE packet\n");
      free(buf);
      return 0;
    }

    *nl = '\0';
    const char *name = buf + 5;
    const char *filedata = nl + 1;
    size_t data_len = body_len - (size_t)(filedata - buf);

    int fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
      perror("open for write");
    } else {
      write(fd, filedata, data_len);
      close(fd);
      printf("[file] \"%s\" received (%zu bytes)\n", name, data_len);
      fflush(stdout);
    }
  }

  free(buf);
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s <username> [server_ip]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  snprintf(username, sizeof(username), "%s", argv[1]);
  const char *server_ip = (argc == 3) ? argv[2] : "127.0.0.1";

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serv_addr;
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
    fprintf(stderr, "invalid address: %s\n", server_ip);
    close(sock);
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("connect");
    close(sock);
    exit(EXIT_FAILURE);
  }
  printf("connected to %s:%d as \"%s\"\n", server_ip, PORT, username);

  struct pollfd fds[2];
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  fds[1].fd = sock;
  fds[1].events = POLLIN;

  char line[4096];

  while (1) {
    int ret = poll(fds, 2, -1);
    if (ret < 0) {
      if (errno == EINTR)
        continue;
      perror("poll");
      break;
    }

    if (fds[0].revents & POLLIN) {
      if (fgets(line, sizeof(line), stdin) == NULL)
        break;
      if (handle_send(sock, line) < 0)
        break;
    }

    if (fds[1].revents & POLLIN) {
      if (handle_receive(sock) < 0) {
        printf("server disconnected\n");
        break;
      }
    }
  }

  close(sock);
  return 0;
}