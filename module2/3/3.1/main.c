#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void print_mode_info(mode_t mode) {
  printf(" Binary: ");
  for (int i = 8; i >= 0; i--) {
    printf("%d", (mode & (1 << i) ? 1 : 0));
  }
  printf("\n");
  printf(" Digital: %03o\n", mode);
  printf(" In letters: ");
  printf((mode & S_IRUSR) ? "r" : "-");
  printf((mode & S_IWUSR) ? "w" : "-");
  printf((mode & S_IXUSR) ? "x" : "-");
  printf((mode & S_IRGRP) ? "r" : "-");
  printf((mode & S_IWGRP) ? "w" : "-");
  printf((mode & S_IXGRP) ? "x" : "-");
  printf((mode & S_IROTH) ? "r" : "-");
  printf((mode & S_IWOTH) ? "w" : "-");
  printf((mode & S_IXOTH) ? "x" : "-");
  printf("\n");
}

int main() {
  char buffer[256];
  mode_t current_mode = 0;
  int mode_set = 0;

  while (1) {
    draw_header("Rights Acess Utility", TXT_GREEN);

    if (mode_set) {
      printf("\n Current rights:\n");
      print_mode_info(current_mode);
    } else {
      printf("\n Rights not set\n");
    }

    printf("\n");
    printf("╭───────────────────────────────────────╮\n");
    printf("│ [1] Enter rights                      │\n");
    printf("│ [2] Read file rights                  │\n");
    printf("│ [3] Change rights                     │\n");
    printf("│ [4] Exit                              │\n");
    printf("╰───────────────────────────────────────╯\n");
    printf(" Choose an option: ");

    read_input(buffer, sizeof(buffer));

    if (strcmp(buffer, "1") == 0) {
      printf("\n Enter rights (e.g. 0755 or rwxr-xr-x): ");
      read_input(buffer, sizeof(buffer));

      if (strlen(buffer) == 9 && (buffer[0] == 'r' || buffer[0] == '-')) {
        current_mode = 2;
        // current_mode = parse_string_mode(buffer);
        mode_set = 1;
      } else {
        char *endptr;
        long val = strtol(buffer, &endptr, 8);

        if (*endptr == '\0') {
          current_mode = (mode_t)val;
          mode_set = 1;
        } else {
          printf(" Rights entered wrong");
          wait_enter();
        }
      }

    } else if (strcmp(buffer, "2") == 0) {
    } else if (strcmp(buffer, "3") == 0) {
    } else if (strcmp(buffer, "4") == 0) {
      break;
    } else {
      printf(TXT_RED "\n Wrong choice.\n" RESET);
      wait_enter();
    }
  }
  return 0;
}