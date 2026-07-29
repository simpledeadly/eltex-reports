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

void print_ip(uint32_t ip) {
  printf("%u.%u.%u.%u", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF,
         ip & 0xFF);
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: %s <ip_gateway> <subnet_mask> <packet_quantity>\n", argv[0]);
    return 1;
  }

  uint32_t gw = parse_ip(argv[1]);
  uint32_t mask = parse_ip(argv[2]);
  int count = atoi(argv[3]);

  if (count <= 0) {
    printf("Packet quantity must be greater than 0.\n");
    return 1;
  }

  uint32_t network_addr = gw & mask;

  srand(time(NULL));

  int own_count = 0;
  int other_count = 0;

  for (int i = 0; i < count; i++) {
    uint32_t random_ip =
        ((uint32_t)(rand() & 0xFF) << 24 | (uint32_t)(rand() & 0xFF) << 16 |
         (uint32_t)(rand() & 0xFF) << 8 | (uint32_t)(rand() & 0xFF));

    if ((random_ip & mask) == network_addr) {
      own_count++;
    } else {
      other_count++;
    }
  }

  printf("\n———————— Statistics ————————\n");
  printf("Total packets: %d\n", count);
  printf("Gateway IP:     ");
  print_ip(gw);
  printf("\n");
  printf("Subnet mask:    ");
  print_ip(mask);
  printf("\n");
  printf("Network ADDR:   ");
  print_ip(network_addr);
  printf("\n");
  printf("Local subnet:   %d (%.2f%%)\n", own_count,
         (double)own_count / count * 100.0);
  printf("Other subnets:  %d (%.2f%%)\n", other_count,
         (double)other_count / count * 100.0);
  printf("————————————————————————————\n");

  return 0;
}
