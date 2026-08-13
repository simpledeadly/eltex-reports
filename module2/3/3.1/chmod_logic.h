#ifndef CHMOD_LOGIC_H
#define CHMOD_LOGIC_H

#include <sys/types.h>

mode_t parse_string_mode(const char *str);
mode_t apply_chmod(mode_t mode, const char *mod_str);

#endif
