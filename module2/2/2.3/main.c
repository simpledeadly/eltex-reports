#include "tui.h"
#include <stdio.h>
#include <stdlib.h>

double add(double a, double b) { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b) { return a / b; }

int main() {
  char input_buf[20];
  int choice;
  double a, b;
  double (*operations[4])(double, double) = {add, subtract, multiply, divide};

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
    if (choice == 4 && b == 0) {
      printf("Infinity! Or it's just impossible.\n");
    } else {
      printf("%.2f\n", operations[choice - 1](a, b));
    }

    wait_enter();
  }
}
