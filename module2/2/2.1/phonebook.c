#include "phonebook.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contact_count = 0;

struct Contact phonebook[100];

int add_contact_record(struct Contact c) {
  if (contact_count >= 100)
    return -1;
  phonebook[contact_count] = c;
  contact_count++;
  return 0;
}

int edit_contact_record(int index, struct Contact c) {
  if (index < 0 || index >= contact_count)
    return -1;
  phonebook[index] = c;
  return 0;
}

int delete_contact_record(int index) {
  if (index < 0 || index >= contact_count)
    return -1;
  for (int i = index; i < contact_count - 1; i++) {
    phonebook[i] = phonebook[i + 1];
  }
  contact_count--;
  return 0;
}

void show_contacts() {
  char title_buffer[50];
  snprintf(title_buffer, 50, "All Contacts (Total: %d)", contact_count);

  draw_header(title_buffer, TXT_GREEN);

  if (contact_count == 0) {
    printf(" No contacts.\n");
    return;
  }

  for (int i = 0; i < contact_count; i++) {
    printf(" ╭──────────────────────────────────────╮\n");
    printf(" │ Contact ID: %-24d │\n", i + 1);
    printf(" ├──────────────────────────────────────┤\n");

    printf(" │ Name:     %-26s │\n", phonebook[i].name);
    printf(" │ Surname:  %-26s │\n", phonebook[i].surname);

    if (strlen(phonebook[i].phone) > 0) {
      printf(" │ Phone:    %-26s │\n", phonebook[i].phone);
    }
    if (strlen(phonebook[i].email) > 0) {
      printf(" │ Email:    %-26s │\n", phonebook[i].email);
    }
    if (strlen(phonebook[i].gh_link) > 0) {
      printf(" │ GitHub:   %-26s │\n", phonebook[i].gh_link);
    }
    if (strlen(phonebook[i].job_role) > 0) {
      printf(" │ Role:     %-26s │\n", phonebook[i].job_role);
    }
    if (strlen(phonebook[i].employer) > 0) {
      printf(" │ Employer: %-26s │\n", phonebook[i].employer);
    }

    printf(" ╰──────────────────────────────────────╯\n");
  }
}

void add_contact() {
  draw_header("New contact", TXT_GREEN);

  struct Contact c;
  memset(&c, 0, sizeof(c));
  char buffer[50];

  while (1) {
    printf(" Enter name (required): ");
    read_input(buffer, 50);
    if (strlen(buffer) > 0)
      break;
  }
  strcpy(c.name, buffer);

  while (1) {
    printf(" Enter surname (required): ");
    read_input(buffer, 50);
    if (strlen(buffer) > 0)
      break;
  }
  strcpy(c.surname, buffer);

  printf(" Enter phone number (optional): ");
  read_input(buffer, 50);
  strcpy(c.phone, buffer);

  printf(" Enter email (optional): ");
  read_input(buffer, 50);
  strcpy(c.email, buffer);

  printf(" Enter GitHub link (optional): ");
  read_input(buffer, 50);
  strcpy(c.gh_link, buffer);

  printf(" Enter job role (optional): ");
  read_input(buffer, 50);
  strcpy(c.job_role, buffer);

  printf(" Enter employer (optional): ");
  read_input(buffer, 50);
  strcpy(c.employer, buffer);

  if (add_contact_record(c) == 0) {
    printf(TXT_GREEN "\nContact successfully saved!" RESET "\n");
  } else {
    printf(TXT_RED "\nPhonebook is full!" RESET "\n");
  }
}

void edit_contact() {
  draw_header("Contact editting", TXT_GREEN);

  if (contact_count == 0) {
    printf(" Add contacts first.\n");
    return;
  }

  show_contacts();

  char buffer[50];
  printf("\n Enter contact ID for editting: ");
  read_input(buffer, 5);

  int id = atoi(buffer);
  if (id < 1 || id > contact_count) {
    printf(TXT_RED " No such ID." RESET "\n");
    return;
  }

  int index = id - 1;
  struct Contact c = phonebook[index];
  draw_header("Editing...", TXT_GREEN);

  printf(" Old name: %s\n", c.name);
  printf(" Enter new name (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0)
    strcpy(c.name, buffer);

  printf(" Old surname: %s\n", c.surname);
  printf(" Enter new surname (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0)
    strcpy(c.surname, buffer);

  printf(" Old phone: %s\n", c.phone);
  printf(" Enter new phone (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0)
    strcpy(c.phone, buffer);

  printf(" Old gh_link: %s\n", c.gh_link);
  printf(" Enter new gh_link (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0)
    strcpy(c.gh_link, buffer);

  printf(" Old job role: %s\n", c.job_role);
  printf(" Enter new job role (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0)
    strcpy(c.job_role, buffer);

  printf(" Old employer: %s\n", c.employer);
  printf(" Enter new employer (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0)
    strcpy(c.employer, buffer);

  if (edit_contact_record(index, c) == 0) {
    printf(TXT_GREEN "\n Contact successfully updated!" RESET "\n");
  } else {
    printf(TXT_RED "\n Failed to update." RESET "\n");
  }
}

void delete_contact() {
  draw_header("Contact deletion", TXT_RED);

  if (contact_count == 0) {
    printf(" Add contacts first.\n");
    return;
  }

  show_contacts();

  printf("\n Enter contact ID for deletion: ");

  char buffer[5];
  read_input(buffer, 5);
  int id = atoi(buffer);

  if (id < 1 || id > contact_count) {
    printf(TXT_RED " No such ID." RESET "\n");
    return;
  }

  int index = id - 1;
  if (delete_contact_record(index) == 0) {
    printf(TXT_GREEN "\n Contact deleted." RESET "\n");
  } else {
    printf(TXT_RED "\n Failed to delete." RESET "\n");
  }
}