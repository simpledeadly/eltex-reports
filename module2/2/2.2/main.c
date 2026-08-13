#include "calc.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  char input_buf[20];
  int choice;
  double a, b;

  while (1) {
    show_menu();

    read_input(input_buf, 20);
    choice = atoi(input_buf);

    if (choice == 5) {
      system("clear");
      return 0;
    }

    if (choice < 1 || choice > 4) {
      continue;
    }

    system("clear");
    draw_header("Calculating...", TXT_GREEN);

    printf(" Enter first number: ");
    read_input(input_buf, 20);
    a = atof(input_buf);

    printf(" Enter second number: ");
    read_input(input_buf, 20);
    b = atof(input_buf);

    printf(TXT_GREEN "\n Result: " RESET);
    int error = 0;
    switch (choice) {
    case 1:
      printf("%.2f\n", calc_add(a, b));
      break;
    case 2:
      printf("%.2f\n", calc_sub(a, b));
      break;
    case 3:
      printf("%.2f\n", calc_mul(a, b));
      break;
    case 4: {
      double res = calc_div(a, b, &error);
      if (error) {
        printf("Infinity! Or it's just impossible.\n");
      } else {
        printf("%.2f\n", res);
      }
    } break;
    }

    wait_enter();
  }
}
