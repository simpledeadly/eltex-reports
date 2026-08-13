#include "phonebook.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Contact *head = NULL;

Contact *get_head() { return head; }

int add_contact_record(const char *name, const char *phone) {
  Contact *node = malloc(sizeof(Contact));
  if (node == NULL)
    return -1;

  strncpy(node->name, name, sizeof(node->name) - 1);
  node->name[sizeof(node->name) - 1] = '\0';

  strncpy(node->phone, phone, sizeof(node->phone) - 1);
  node->phone[sizeof(node->phone) - 1] = '\0';

  node->next = NULL;
  node->prev = NULL;

  if (head == NULL) {
    head = node;
    return 0;
  }

  Contact *current = head;
  Contact *prev = NULL;

  // Вставка с сохранением лексикографического порядка по имени
  while (current != NULL && strcmp(current->name, node->name) < 0) {
    prev = current;
    current = current->next;
  }

  if (prev == NULL) {
    // Вставка в начало
    node->next = head;
    head->prev = node;
    head = node;
  } else {
    // Вставка в середину или конец
    node->next = current;
    node->prev = prev;
    prev->next = node;
    if (current != NULL) {
      current->prev = node;
    }
  }
  return 0;
}

int delete_contact_record(int index) {
  if (head == NULL || index < 1)
    return -1;

  Contact *current = head;
  for (int i = 1; i < index && current != NULL; i++) {
    current = current->next;
  }

  if (current == NULL)
    return -1;

  if (current->prev != NULL) {
    current->prev->next = current->next;
  } else {
    head = current->next;
  }

  if (current->next != NULL) {
    current->next->prev = current->prev;
  }

  free(current);
  return 0;
}

int edit_contact_record(int index, const char *new_name,
                        const char *new_phone) {
  if (delete_contact_record(index) != 0)
    return -1;
  return add_contact_record(new_name, new_phone);
}

void clear_phonebook_records() {
  Contact *current = head;
  while (current != NULL) {
    Contact *next = current->next;
    free(current);
    current = next;
  }
  head = NULL;
}

void add_contact() {
  char name[64];
  char phone[32];

  printf("Name: ");
  read_input(name, sizeof(name));

  printf("Phone: ");
  read_input(phone, sizeof(phone));

  if (add_contact_record(name, phone) == 0) {
    printf("Added successfully.\n");
  } else {
    printf("Failed to add.\n");
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

  if (delete_contact_record(n) == 0) {
    printf("Deleted.\n");
  } else {
    printf("Invalid number.\n");
  }
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

  char name[64];
  char phone[32];

  printf("New name (old: %s): ", current->name);
  read_input(name, sizeof(name));
  if (strlen(name) == 0)
    strcpy(name, current->name);

  printf("New phone (old: %s): ", current->phone);
  read_input(phone, sizeof(phone));
  if (strlen(phone) == 0)
    strcpy(phone, current->phone);

  if (edit_contact_record(n, name, phone) == 0) {
    printf("Updated.\n");
  } else {
    printf("Update failed.\n");
  }
}

void clear_phonebook() { clear_phonebook_records(); }