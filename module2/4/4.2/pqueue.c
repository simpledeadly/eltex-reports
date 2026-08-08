#include "pqueue.h"
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