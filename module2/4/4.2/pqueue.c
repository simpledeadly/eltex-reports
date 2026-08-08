#include "pqueue.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Node *head = NULL;

void enqueue(const char *data, uint8_t priority) {
  Node *node = malloc(sizeof(Node));
  if (node == NULL) {
    return;
  }

  strncpy(node->data, data, sizeof(node->data) - 1);
  node->data[sizeof(node->data) - 1] = '\0';
  node->priority = priority;
  node->next = NULL;

  Node *cur = head;

  if (head == NULL || priority > head->priority) {
    node->next = head;
    head = node;
    return;
  }

  while (cur->next != NULL && cur->next->priority >= priority) {
    cur = cur->next;
  }
  node->next = cur->next;
  cur->next = node;
}

int dequeue_first(char *out_data, uint8_t *out_prio) {
  if (head == NULL) {
    return 1;
  }
  strncpy(out_data, head->data, sizeof(head->data) - 1);
  out_data[sizeof(head->data) - 1] = '\0';
  *out_prio = head->priority;
  Node *next = head->next;
  free(head);
  head = next;
  return 0;
}

int dequeue_exact_priority(uint8_t equal_prio, char *out_data,
                           uint8_t *out_prio) {
  if (head == NULL) {
    return 1;
  }
  Node *prev = NULL;
  Node *cur = head;

  while (cur != NULL) {
    if (cur->priority == equal_prio) {
      break;
    }
    prev = cur;
    cur = cur->next;
  }

  if (cur == NULL) {
    return 1;
  }

  strncpy(out_data, cur->data, sizeof(cur->data) - 1);
  out_data[sizeof(cur->data) - 1] = '\0';
  *out_prio = cur->priority;

  if (prev == NULL) {
    head = cur->next;
  } else {
    prev->next = cur->next;
  }

  free(cur);
  return 0;
}

int dequeue_ge_priority(uint8_t min_priority, char *out_data,
                        uint8_t *out_prio) {
  if (head == NULL || head->priority < min_priority) {
    return 1;
  }
  strncpy(out_data, head->data, sizeof(head->data) - 1);
  out_data[sizeof(head->data) - 1] = '\0';
  *out_prio = head->priority;
  Node *next = head->next;
  free(head);
  head = next;
  return 0;
}

void clear_queue(void) {
  Node *cur = head;
  while (cur != NULL) {
    Node *next = cur->next;
    free(cur);
    cur = next;
  }
  head = NULL;
}

void print_queue() {
  Node *cur = head;
  while (cur != NULL) {
    printf("[%u] %s\n", cur->priority, cur->data);
    cur = cur->next;
  }
}