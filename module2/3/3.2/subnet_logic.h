#ifndef SUBNET_LOGIC_H
#define SUBNET_LOGIC_H

#include <stdint.h>

uint32_t parse_ip(const char *ip_str);
uint32_t get_network_addr(uint32_t ip, uint32_t mask);
uint32_t get_broadcast_addr(uint32_t ip, uint32_t mask);
uint32_t get_hosts_count(uint32_t mask);
int is_in_subnet(uint32_t ip, uint32_t network_addr, uint32_t mask);

#endif
