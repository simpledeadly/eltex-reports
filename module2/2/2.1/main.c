#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int choice;
int contact_count = 0;

struct Contact {
  char name[50];
  char surname[50];
  char email[50];
  char gh_link[50];
  char job_role[50];
  char employer[50];
};

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

int main() {
  while (1) {
    show_menu();
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      system("clear");
      show_contacts();
      wait_enter();
      break;
    case 2:
      system("clear");
      add_contact();
      wait_enter();
      break;
    case 3:
      system("clear");
      draw_header("Contact Editing", NULL);
      wait_enter();
      break;
    case 4:
      system("clear");
      printf(TXT_RED "Контакт удален. Нажмите Enter для возврата в меню...\n");
      wait_enter();
      break;
    case 5:
      system("clear");
      return 0;
      break;
    default:
      system("clear");
      printf("No such option/wrong input, try again!\n\n");
      wait_enter();
      break;
    }
  };
  return 0;
};