#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void draw_header(const char *title, const char *color) {
  system("clear");
  const char *applied_color = color != NULL ? color : RESET;
  printf("%s╭───────────────────────────────────────╮\n", applied_color);
  printf("│" RESET " %-37s %s│\n", title, applied_color);
  printf("╰───────────────────────────────────────╯\n" RESET);
}

void wait_enter(void) {
  printf("\nНажмите Enter для продолжения...");
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void read_input(char *buffer, int max_len) {
  if (fgets(buffer, max_len, stdin) != NULL) {
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    } else {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
    }
  }
}
