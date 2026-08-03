#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *fifo_name = NULL;
  int opt;

  while ((opt = getopt(argc, argv, "p:")) != -1) {
    if (opt == 'p') {
      fifo_name = optarg;
    } else {
      fprintf(stderr,
              "Using: %s [-p fifo_name] file1 file2 ...\nTwo files at least\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (optind >= argc) {
    fprintf(stderr, "Enter files to copy.\n");
    fprintf(stderr,
            "Using: %s [-p fifo_name] file1 file2 ...\nTwo files at least\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  return 0;
}