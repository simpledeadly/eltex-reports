#include "phonebook.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int choice;
char input_buf[5];

int main() {
  while (1) {
    show_menu();

    read_input(input_buf, 5);
    choice = atoi(input_buf);

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
      edit_contact();
      wait_enter();
      break;
    case 4:
      system("clear");
      delete_contact();
      wait_enter();
      break;
    case 5:
      system("clear");
      return 0;
      break;
    default:
      break;
    }
  };
  return 0;
};