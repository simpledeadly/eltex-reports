#include "tui.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Plugin {
  char name[32];
  double (*func)(double, double);
  void *handle;
} Plugin;

Plugin plugins[10];
int plugin_count = 0;

void load_plugins() {
  DIR *dir = opendir("./plugins");
  if (dir == NULL) {
    printf("Could not open plugins directory.\n");
    return;
  }

  struct dirent *ent;

  while ((ent = readdir(dir)) != NULL) {
    if (strstr(ent->d_name, ".dylib") == NULL) {
      continue;
    }

    char path[256];
    snprintf(path, sizeof(path), "./plugins/%s", ent->d_name);

    void *handle = dlopen(path, RTLD_LAZY);
    if (handle == NULL) {
      printf("Loading error %s: %s\n", ent->d_name, dlerror());
      continue;
    }

    char func_name[32] = {0};
    strncpy(func_name, ent->d_name + 3, strlen(ent->d_name) - 9);

    double (*func_ptr)(double, double) = dlsym(handle, func_name);

    if (func_ptr == NULL || plugin_count >= 10) {
      dlclose(handle);
      continue;
    }

    strncpy(plugins[plugin_count].name, func_name, 31);
    plugins[plugin_count].func = func_ptr;
    plugins[plugin_count].handle = handle;
    plugin_count++;
  }
}

void unload_plugins() {
  for (int i = 0; i < plugin_count; i++) {
    dlclose(plugins[i].handle);
  }
}

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
