#include "pqueue.h"
#include <stdio.h>

int main() {
  enqueue("msg1", 50);
  enqueue("msg2", 200);
  enqueue("msg3", 10);
  enqueue("msg4", 200);

  printf("Queue after enqueue:\n");
  print_queue();

  char buf[64];
  uint8_t prio;

  dequeue_first(buf, &prio);
  printf("dequeue_first: %s (%u)\n", buf, prio);

  dequeue_exact_priority(10, buf, &prio);
  printf("dequeue_exact_priority(10): %s (%u)\n", buf, prio);

  dequeue_ge_priority(100, buf, &prio);
  printf("dequeue_ge_priority(100): %s (%u)\n", buf, prio);

  print_queue();
  clear_queue();
  return 0;
}