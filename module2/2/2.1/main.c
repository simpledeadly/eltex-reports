#include "phonebook.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int choice;

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