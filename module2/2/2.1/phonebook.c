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
  getchar();

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