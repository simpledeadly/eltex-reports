#ifndef PHONEBOOK_H
#define PHONEBOOK_H

// самореферентная структура
typedef struct Contact {
  char name[64];
  char phone[32];
  struct Contact *next;
  struct Contact *prev;
} Contact;

void show_contacts();
void add_contact();
void edit_contact();
void delete_contact();
void clear_phonebook();

#endif