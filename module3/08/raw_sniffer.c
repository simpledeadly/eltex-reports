#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define CHAT_PORT 12345
#define DNS_PORT 53

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <chat|dns>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int target_port;
  if (strcmp(argv[1], "chat") == 0)
    target_port = CHAT_PORT;
  else if (strcmp(argv[1], "dns") == 0)
    target_port = DNS_PORT;
  else {
    fprintf(stderr, "unknown filter\n");
    exit(EXIT_FAILURE);
  }

  int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sock == -1) {
    perror("socket (need root: sudo ./raw_sniffer <chat|dns>)");
    exit(EXIT_FAILURE);
  }

  printf("sniffer: capturing UDP port %d, Ctrl+C to stop\n", target_port);

  time_t start = time(NULL);
  unsigned char buf[65536];

  while (1) {
    ssize_t len = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);
    if (len <= 0)
      continue;

    if (len < 14)
      continue;
    unsigned char *dst_mac = buf;
    unsigned char *src_mac = buf + 6;
    uint16_t ethertype = ntohs(*(uint16_t *)(buf + 12));
    if (ethertype != ETHERTYPE_IP)
      continue;

    struct ip *iph = (struct ip *)(buf + 14);
    if (iph->ip_p != IPPROTO_UDP) continue;
    int ip_header_len = iph->ip_hl * 4;
    
    struct udphdr *udph = (struct udphdr *)(buf + 14 + ip_header_len);
    uint16_t sport = ntohs(udph->uh_sport);
    uint16_t dport = ntohs(udph->uh_dport);

    if (sport != target_port && dport != target_port)
      continue;

    char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &iph->ip_src, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET, &iph->ip_dst, dst_ip, sizeof(dst_ip));

    printf("[t=%.0fs] MAC %02x:%02x:%02x:%02x:%02x:%02x -> "
           "%02x:%02x:%02x:%02x:%02x:%02x | IP %s:%d -> %s:%d\n",
           difftime(time(NULL), start), src_mac[0], src_mac[1], src_mac[2],
           src_mac[3], src_mac[4], src_mac[5], dst_mac[0], dst_mac[1],
           dst_mac[2], dst_mac[3], dst_mac[4], dst_mac[5], src_ip, sport,
           dst_ip, dport);
  }

  close(sock);
  return 0;
}