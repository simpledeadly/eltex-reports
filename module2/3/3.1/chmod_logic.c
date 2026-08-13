#include "chmod_logic.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

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
    // No operator
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
