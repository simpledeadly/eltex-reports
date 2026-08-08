#include "phonebook.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Contact *head = NULL;

void add_contact() {
  Contact *node = malloc(sizeof(Contact));
  if (node == NULL) {
    printf("Memory allocation failed.");
    return;
  }
  printf("Name: ");
  fgets(node->name, sizeof(node->name), stdin);
  node->name[strcspn(node->name, "\n")] = '\0';

  printf("Phone: ");
  fgets(node->phone, sizeof(node->phone), stdin);
  node->phone[strcspn(node->phone, "\n")] = '\0';

  node->next = NULL;
  node->prev = NULL;

  if (head == NULL) {
    head = node;
  } else {
    Contact *current = head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = node;
    node->prev = current;
  }
}

void show_contacts() {
  if (head == NULL) {
    printf("Phonebook is empty.\n");
    return;
  }
  Contact *current = head;
  int i = 1;
  while (current != NULL) {
    printf("[%d] %s — %s\n", i, current->name, current->phone);
    current = current->next;
    i++;
  }
}

void edit_contact(void) {}
void delete_contact(void) {}
void clear_phonebook(void) {}
