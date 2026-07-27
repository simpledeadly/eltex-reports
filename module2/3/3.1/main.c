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

mode_t parse_string_mode(const char *str) {
  mode_t mode = 0;

  if (strlen(str) >= 9) {
    if (str[0] == 'r')
      mode |= S_IRUSR;
    if (str[1] == 'w')
      mode |= S_IWUSR;
    if (str[2] == 'x')
      mode |= S_IXUSR;
    if (str[3] == 'r')
      mode |= S_IRGRP;
    if (str[4] == 'w')
      mode |= S_IWGRP;
    if (str[5] == 'x')
      mode |= S_IXGRP;
    if (str[6] == 'r')
      mode |= S_IROTH;
    if (str[7] == 'w')
      mode |= S_IWOTH;
    if (str[8] == 'x')
      mode |= S_IXOTH;
  }

  return mode;
}

mode_t apply_chmod(mode_t mode, const char *mod_str) {
  mode_t target_mask = 0;
  mode_t perm_mask = 0;
  char op = 0;
  int i = 0;

  // define for whom we wanna change rights (u/g/o/a)
  while (mod_str[i] != '\0' && strchr("ugoa", mod_str[i])) {
    if (mod_str[i] == 'u')
      target_mask |= 0700;
    if (mod_str[i] == 'g')
      target_mask |= 0070;
    if (mod_str[i] == 'o')
      target_mask |= 0007;
    if (mod_str[i] == 'a')
      target_mask |= 0777;
    i++;
  }
  if (target_mask == 0) {
    target_mask = 0777;
  }

  if (mod_str[i] == '+' || mod_str[i] == '-' || mod_str[i] == '=') {
    op = mod_str[i];
    i++;
  } else {
    printf(" No operator (+, -, =)\n");
    return mode;
  }

  // rights (r,w,x)
  while (mod_str[i] != '\0') {
    if (mod_str[i] == 'r')
      perm_mask |= 0444;
    if (mod_str[i] == 'w')
      perm_mask |= 0222;
    if (mod_str[i] == 'x')
      perm_mask |= 0111;
    i++;
  }

  mode_t apply_mask = target_mask & perm_mask;

  if (op == '+') {
    mode |= apply_mask;
  } else if (op == '-') {
    mode &= ~apply_mask;
  } else if (op == '=') {
    mode &= ~target_mask;
    mode |= apply_mask;
  }

  return mode;
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
        current_mode = parse_string_mode(buffer);
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
      printf("\n Enter filename: ");
      read_input(buffer, sizeof(buffer));

      struct stat st;
      if (stat(buffer, &st) == 0) {
        current_mode = st.st_mode & 0777;
        mode_set = 1;

        printf("\n stat() says:\n");
        print_mode_info(current_mode);

        printf("\n Comparing with " TXT_GREEN "`ls -l`" RESET ":\n");
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "ls -l \"%s\"", buffer);
        system(cmd);

        wait_enter();
      }
    } else if (strcmp(buffer, "3") == 0) {
      if (!mode_set) {
        printf(TXT_RED "\n Firstly set rights (try 1 or 2 options).\n" RESET);
        wait_enter();
        continue;
      }

      printf("\n Enter changes (e.g. u+x, a=rw, g-w): ");
      read_input(buffer, sizeof(buffer));

      current_mode = apply_chmod(current_mode, buffer);
      printf("\n New rights:\n");
      print_mode_info(current_mode);

      wait_enter();
    } else if (strcmp(buffer, "4") == 0) {
      break;
    } else {
      printf(TXT_RED "\n There is no a such option.\n" RESET);
      wait_enter();
    }
  }
  return 0;
}