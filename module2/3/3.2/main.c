#include "subnet_logic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

  if (gw == 0 || mask == 0) {
    printf("Wrong IP format.\n");
    return 1;
  }

  if (count <= 0) {
    printf("Packet quantity must be greater than 0.\n");
    return 1;
  }

  uint32_t network_addr = get_network_addr(gw, mask);

  srand(time(NULL));

  int own_count = 0;
  int other_count = 0;

  for (int i = 0; i < count; i++) {
    uint32_t random_ip =
        ((uint32_t)(rand() & 0xFF) << 24 | (uint32_t)(rand() & 0xFF) << 16 |
         (uint32_t)(rand() & 0xFF) << 8 | (uint32_t)(rand() & 0xFF));

    if (is_in_subnet(random_ip, network_addr, mask)) {
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
