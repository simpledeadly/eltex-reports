#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void wait_enter() {
  printf("\nPress Enter to continue...");
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
  printf("│ Calculator                            │\n");
  printf("│───────────────────────────────────────│\n");
  printf("│ [1] Addition                          │\n");
  printf("│ [2] Subtraction                       │\n");
  printf("│ [3] Multiplication                    │\n");
  printf("│ [4] Division                          │\n");
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