#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TXT_RED "\x1b[31m"
#define TXT_GREEN "\x1b[32m"
#define BG_BLUE "\x1b[44m"
#define RESET "\x1b[0m"

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

void wait_enter() {
  printf("\nPress Enter to continue...");
  getchar();
  getchar();
  system("clear");
}

void draw_header(const char *title, const char *color) {
  system("clear");

  const char *applied_color = color != NULL ? color : RESET;

  printf("%s╭───────────────────────────────────────╮\n", applied_color);
  printf("│" RESET " %-37s %s│\n", title, applied_color);
  // printf("│───────────────────────────────────────│\n" RESET);
  printf("╰───────────────────────────────────────╯\n" RESET);
}

void show_menu() {
  system("clear");
  printf("╭───────────────────────────────────────╮\n");
  printf("│ Phonebook                             │\n");
  printf("│───────────────────────────────────────│\n");
  printf("│ [1] Show all contacts                 │\n");
  printf("│ [2] Add a contact                     │\n");
  printf("│ [3] Edit a contact                    │\n");
  printf("│ [4] Delete a contact                  │\n");
  printf("│ [5] Quit                              │\n");
  printf("╰───────────────────────────────────────╯\n");
  printf(" Type option number: ");
};

void read_input(char *buffer, int max_len) {
  fgets(buffer, max_len, stdin);
  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
  }
}

void show_contacts() {
  draw_header("All Contacts", TXT_GREEN);

  if (contact_count == 0) {
    printf(" No contacts.\n");
    return;
  }

  for (int i = 0; i < contact_count; i++) {
    printf(" ╭──────────────────────────────────────╮\n");
    // Вывод ID контакта (выравнивание цифры с помощью %-25d)
    printf(" │ Contact ID: %-24d │\n", i + 1);
    printf(" ├──────────────────────────────────────┤\n");

    // Обязательные поля выводятся всегда
    printf(" │ Name:     %-26s │\n", phonebook[i].name);
    printf(" │ Surname:  %-26s │\n", phonebook[i].surname);

    // Необязательные поля проверяются на пустоту перед выводом
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