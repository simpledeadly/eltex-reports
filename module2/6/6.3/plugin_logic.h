#ifndef PLUGIN_LOGIC_H
#define PLUGIN_LOGIC_H

typedef struct Plugin {
  char name[32];
  double (*func)(double, double);
  void *handle;
} Plugin;

extern Plugin plugins[10];
extern int plugin_count;

void load_plugins();
void unload_plugins();

#endif
