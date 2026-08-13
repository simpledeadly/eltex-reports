#ifndef PHONEBOOK_H
#define PHONEBOOK_H

typedef struct Contact {
  char name[64];
  char phone[32];
  struct Contact *next;
  struct Contact *prev;
} Contact;

// Чистые функции для тестов и бизнес-логики
Contact *get_head();
int add_contact_record(const char *name, const char *phone);
int delete_contact_record(int index);
int edit_contact_record(int index, const char *new_name, const char *new_phone);
void clear_phonebook_records();

void show_contacts();
void add_contact();
void edit_contact();
void delete_contact();
void clear_phonebook();

#endif