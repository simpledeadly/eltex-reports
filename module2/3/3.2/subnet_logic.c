#include "subnet_logic.h"
#include <stdio.h>
#include <stdlib.h>

uint32_t parse_ip(const char *ip_str) {
  unsigned int bytes[4];
  if (sscanf(ip_str, "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2],
             &bytes[3]) != 4) {
    return 0; // return 0 on error for tests
  }
  return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]);
}

uint32_t get_network_addr(uint32_t ip, uint32_t mask) { return ip & mask; }

uint32_t get_broadcast_addr(uint32_t ip, uint32_t mask) {
  return (ip & mask) | (~mask);
}

uint32_t get_hosts_count(uint32_t mask) {
  uint32_t inverted_mask = ~mask;
  if (inverted_mask == 0)
    return 0; // /32
  if (inverted_mask == 1)
    return 0; // /31
  return inverted_mask - 1;
}

int is_in_subnet(uint32_t ip, uint32_t network_addr, uint32_t mask) {
  return (ip & mask) == network_addr;
}
