#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 12345
#define BUF_SIZE 1024
#define BROADCAST_ADDR "192.168.0.255"
// #define BROADCAST_ADDR "127.255.255.255"

static int sock;
static struct sockaddr_in broadcast_addr;
static char username[32];

static void send_msg(const char *text) {
  char buf[BUF_SIZE];
  snprintf(buf, sizeof(buf), "%s", text);
  if (sendto(sock, buf, strlen(buf) + 1, 0, (struct sockaddr *)&broadcast_addr,
             sizeof(broadcast_addr)) == -1) {
    perror("sendto");
  }
}

static void announce(const char *event) {
  char msg[BUF_SIZE];
  snprintf(msg, sizeof(msg), "%s %s", username, event);
  send_msg(msg);
}

static void on_sigint(int sig) {
  (void)sig;
  announce("left the chat");
  _exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <username>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  snprintf(username, sizeof(username), "%s", argv[1]);

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  int broadcast_enable = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_enable,
                 sizeof(broadcast_enable)) < 0) {
    perror("setsockopt SO_BROADCAST");
    close(sock);
    exit(EXIT_FAILURE);
  }

  int reuse = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    perror("setsockopt SO_REUSEADDR");
    close(sock);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in my_addr;
  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
    perror("bind");
    close(sock);
    exit(EXIT_FAILURE);
  }

  memset(&broadcast_addr, 0, sizeof(broadcast_addr));
  broadcast_addr.sin_family = AF_INET;
  broadcast_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, BROADCAST_ADDR, &broadcast_addr.sin_addr) <= 0) {
    fprintf(stderr, "Invalid broadcast IP address: %s\n", BROADCAST_ADDR);
    close(sock);
    exit(EXIT_FAILURE);
  }

  signal(SIGINT, on_sigint);
  signal(SIGHUP, on_sigint);

  announce("joined the chat");

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    close(sock);
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    signal(SIGINT, SIG_DFL);
    signal(SIGHUP, SIG_DFL);

    char line[BUF_SIZE - 64];
    while (fgets(line, sizeof(line), stdin) != NULL) {
      line[strcspn(line, "\n")] = '\0';
      char msg[BUF_SIZE];
      snprintf(msg, sizeof(msg), "%s: %s", username, line);
      send_msg(msg);
    }
  } else {
    char buf[BUF_SIZE];
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    while (1) {
      ssize_t n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
                           (struct sockaddr *)&from_addr, &from_len);
      if (n <= 0)
        continue;
      buf[n] = '\0';

      if (strncmp(buf, username, strlen(username)) == 0 &&
          (buf[strlen(username)] == ':' || buf[strlen(username)] == ' '))
        continue;

      char sender_ip[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &from_addr.sin_addr, sender_ip, sizeof(sender_ip));
      int sender_port = ntohs(from_addr.sin_port);

      printf("[%s:%d] %s\n", sender_ip, sender_port, buf);

      fflush(stdout);
    }
  }

  close(sock);
  return 0;
}