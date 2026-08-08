#ifndef TUI
#define TUI

#define TXT_RED "\x1b[31m"
#define TXT_GREEN "\x1b[32m"
#define BG_BLUE "\x1b[44m"
#define RESET "\x1b[0m"

void wait_enter();
void draw_header(const char *title, const char *color);
void show_menu();
void read_input(char *buffer, int max_len);

#endif
