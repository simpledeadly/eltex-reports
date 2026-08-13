#include "plugin_logic.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  load_plugins();

  if (plugin_count == 0) {
    printf("No plugins found in ./plugins directory. Exiting.\n");
    return 1;
  }

  char input_buf[20];
  int choice;
  double a, b;

  while (1) {
    system("clear");
    printf("=== PLUGIN CALCULATOR ===\n");
    for (int i = 0; i < plugin_count; i++) {
      printf("[%d] Operation: %s\n", i + 1, plugins[i].name);
    }
    printf("[%d] Exit\n", plugin_count + 1);
    printf("Choose option: ");
    read_input(input_buf, 20);
    choice = atoi(input_buf);
    if (choice == plugin_count + 1) {
      unload_plugins();
      system("clear");
      return 0;
    }
    if (choice < 1 || choice > plugin_count) {
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

    printf("%.2f\n", plugins[choice - 1].func(a, b));

    wait_enter();
  }
}
