#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint32_t parse_ip(const char *ip_str) {
  unsigned int bytes[4];

  if (sscanf(ip_str, "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2],
             &bytes[3]) != 4) {
    printf("Wrong IP format: %s\n", ip_str);
    exit(1);
  }
  return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]);
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: %s <ip_gateway> <subnet_mask> <packet_quantity>\n", argv[0]);
    return 1;
  }

  return 0;
}
