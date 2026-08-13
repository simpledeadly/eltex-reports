#include "plugin_logic.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

Plugin plugins[10];
int plugin_count = 0;

void load_plugins() {
  plugin_count = 0; // reset for tests
  DIR *dir = opendir("./plugins");
  if (dir == NULL) {
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
      continue;
    }

    char func_name[32] = {0};
    // extract name without lib and .dylib
    // length = strlen - 3 (lib) - 6 (.dylib) = strlen - 9
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
  closedir(dir);
}

void unload_plugins() {
  for (int i = 0; i < plugin_count; i++) {
    dlclose(plugins[i].handle);
  }
  plugin_count = 0;
}
