#ifndef PHONEBOOK_H
#define PHONEBOOK_H

typedef struct Contact {
  char name[64];
  char phone[32];
  struct Contact *left;
  struct Contact *right;
} Contact;

void show_contacts();
void add_contact();
void edit_contact();
void delete_contact();
void clear_phonebook();
void rebalance_tree();

#endif