#ifndef PHONEBOOK
#define PHONEBOOK

struct Contact {
  char name[50];
  char surname[50];
  char phone[50];
  char email[50];
  char gh_link[50];
  char job_role[50];
  char employer[50];
};

void show_contacts();
void add_contact();
void delete_contact();

#endif
