#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdint.h>

typedef struct Node {
  char data[64];
  uint8_t priority;
  struct Node *next;
} Node;

void enqueue(const char *data, uint8_t priority);
int dequeue_first(char *out_data, uint8_t *out_prio);
int dequeue_exact_priority(uint8_t equal_prio, char *out_data,
                           uint8_t *out_prio);
int dequeue_ge_priority(uint8_t min_priority, char *out_data,
                        uint8_t *out_prio);
void clear_queue(void);
void print_queue(void);

#endif