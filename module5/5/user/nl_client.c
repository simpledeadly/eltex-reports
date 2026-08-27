#include <linux/netlink.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define NETLINK_UNIT 31
#define MAX_PAYLOAD 256
#define MSG_TO_KERNEL "hello from userspace"

int main(void) {
  int sock = socket(PF_NETLINK, SOCK_RAW, NETLINK_UNIT);
  if (sock < 0) {
    perror("socket");
    return EXIT_FAILURE;
  }

  struct sockaddr_nl src = {
      .nl_family = AF_NETLINK,
      .nl_pid = getpid(),
  };
  if (bind(sock, (struct sockaddr *)&src, sizeof(src)) < 0) {
    perror("bind");
    close(sock);
    return EXIT_FAILURE;
  }

  struct nlmsghdr *nlh = calloc(1, NLMSG_SPACE(MAX_PAYLOAD));
  if (!nlh) {
    perror("calloc");
    close(sock);
    return EXIT_FAILURE;
  }

  nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
  nlh->nlmsg_pid = getpid();
  nlh->nlmsg_flags = 0;
  strncpy(NLMSG_DATA(nlh), MSG_TO_KERNEL, MAX_PAYLOAD - 1);

  struct sockaddr_nl dst = {
      .nl_family = AF_NETLINK,
      .nl_pid = 0,
      .nl_groups = 0,
  };
  struct iovec iov = {nlh, nlh->nlmsg_len};
  struct msghdr msg = {
      .msg_name = &dst,
      .msg_namelen = sizeof(dst),
      .msg_iov = &iov,
      .msg_iovlen = 1,
  };

  printf("sending: %s\n", MSG_TO_KERNEL);
  if (sendmsg(sock, &msg, 0) < 0) {
    perror("sendmsg");
    free(nlh);
    close(sock);
    return EXIT_FAILURE;
  }

  if (recvmsg(sock, &msg, 0) < 0) {
    perror("recvmsg");
    free(nlh);
    close(sock);
    return EXIT_FAILURE;
  }

  printf("received: %s\n", (char *)NLMSG_DATA(nlh));

  free(nlh);
  close(sock);
  return EXIT_SUCCESS;
}
