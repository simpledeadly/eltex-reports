#include "phonebook.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contact_count = 0;

struct Contact phonebook[100];

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

  char buffer[50];

  while (1) {
    printf(" Enter name (required): ");
    read_input(buffer, 50);
    if (strlen(buffer) > 0)
      break;
  }
  strcpy(phonebook[contact_count].name, buffer);

  while (1) {
    printf(" Enter surname (required): ");
    read_input(buffer, 50);
    if (strlen(buffer) > 0)
      break;
  }
  strcpy(phonebook[contact_count].surname, buffer);

  printf(" Enter phone number (optional): ");
  read_input(buffer, 50);
  strcpy(phonebook[contact_count].phone, buffer);

  printf(" Enter email (optional): ");
  read_input(buffer, 50);
  strcpy(phonebook[contact_count].email, buffer);

  printf(" Enter GitHub link (optional): ");
  read_input(buffer, 50);
  strcpy(phonebook[contact_count].gh_link, buffer);

  printf(" Enter job role (optional): ");
  read_input(buffer, 50);
  strcpy(phonebook[contact_count].job_role, buffer);

  printf(" Enter employer (optional): ");
  read_input(buffer, 50);
  strcpy(phonebook[contact_count].employer, buffer);

  contact_count++;
  printf(TXT_GREEN "\nContact successfully saved!" RESET "\n");
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
  draw_header("Editing...", TXT_GREEN);

  printf(" Old name: %s\n", phonebook[index].name);
  printf(" Enter new name (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0) {
    strcpy(phonebook[index].name, buffer);
  }

  printf(" Old surname: %s\n", phonebook[index].surname);
  printf(" Enter new surname (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0) {
    strcpy(phonebook[index].surname, buffer);
  }

  printf(" Old phone: %s\n", phonebook[index].phone);
  printf(" Enter new phone (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0) {
    strcpy(phonebook[index].phone, buffer);
  }

  printf(" Old gh_link: %s\n", phonebook[index].gh_link);
  printf(" Enter new gh_link (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0) {
    strcpy(phonebook[index].gh_link, buffer);
  }

  printf(" Old job role: %s\n", phonebook[index].job_role);
  printf(" Enter new job role (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0) {
    strcpy(phonebook[index].job_role, buffer);
  }

  printf(" Old employer: %s\n", phonebook[index].employer);
  printf(" Enter new employer (or press Enter to save old): ");
  read_input(buffer, 50);
  if (strlen(buffer) > 0) {
    strcpy(phonebook[index].employer, buffer);
  }

  printf(TXT_GREEN "\n Contact successfully updated!" RESET "\n");
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
  for (int i = index; i < contact_count - 1; i++) {
    phonebook[i] = phonebook[i + 1];
  }

  contact_count--;
  printf(TXT_GREEN "\n Contact deleted." RESET "\n");
}