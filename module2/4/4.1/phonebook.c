#include "phonebook.h"
#include "tui.h"
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
  read_input(node->name, sizeof(node->name));

  printf("Phone: ");
  read_input(node->phone, sizeof(node->phone));

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

void delete_contact() {
  if (head == NULL) {
    printf("Phonebook is empty.\n");
    return;
  }
  show_contacts();
  printf("Enter number to delete: ");

  char buf[8];
  fgets(buf, sizeof(buf), stdin);
  int n = atoi(buf);

  Contact *current = head;
  for (int i = 1; i < n && current != NULL; i++) {
    current = current->next;
  }
  if (current == NULL) {
    printf("Invalid number.\n");
    return;
  }

  if (current->prev != NULL) {
    current->prev->next = current->next;
  } else {
    head = current->next;
  }
  if (current->next != NULL) {
    current->next->prev = current->prev;
  }

  free(current);
  printf("Deleted.\n");
}

void edit_contact() {
  if (head == NULL) {
    printf("Phonebook is empty.\n");
    return;
  }
  show_contacts();
  printf("Enter number to edit: ");

  char buf[8];
  fgets(buf, sizeof(buf), stdin);
  int n = atoi(buf);

  Contact *current = head;
  for (int i = 1; i < n && current != NULL; i++) {
    current = current->next;
  }
  if (current == NULL) {
    printf("Invalid number.\n");
    return;
  }

  printf("New name (old: %s): ", current->name);
  read_input(current->name, sizeof(current->name));

  printf("New phone (old: %s): ", current->phone);
  read_input(current->phone, sizeof(current->phone));

  printf("Updated.\n");
}

void clear_phonebook(void) {}
