#ifndef PHONEBOOK_H
#define PHONEBOOK_H

struct Contact {
  char name[50];
  char surname[50];
  char phone[50];
  char email[50];
  char gh_link[50];
  char job_role[50];
  char employer[50];
};

extern int contact_count;
extern struct Contact phonebook[100];

int add_contact_record(struct Contact c);
int edit_contact_record(int index, struct Contact c);
int delete_contact_record(int index);

void show_contacts();
void add_contact();
void edit_contact();
void delete_contact();

#endif
