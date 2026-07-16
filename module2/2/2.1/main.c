#include <stdio.h>

int choice;

struct Contact {};

void show_menu() {
  printf("+---------------------------------------+\n");
  printf("|          ТЕЛЕФОННАЯ КНИГА             |\n");
  printf("+---------------------------------------+\n");
  printf("|  [1] Добавить контакт                 |\n");
  printf("|  [2] Показать все контакты            |\n");
  printf("|  [3] Редактировать контакт            |\n");
  printf("|  [4] Удалить контакт                  |\n");
  printf("|  [5] Выход                            |\n");
  printf("+---------------------------------------+\n");
  printf(" Ваш выбор: ");
};

int main() {
  while (1) {
    show_menu();
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      printf("Add contact in dev\n\n");
      break;
    case 2:
      printf("Show contacts\n\n");
      break;
    case 3:
      printf("Edit contact\n\n");
      break;
    case 4:
      printf("Delete contact\n\n");
      break;
    case 5:
      return 0;
      break;
    default:
      printf("No such option/wrong input, try again!\n\n");
      break;
    }
  };
  return 0;
};