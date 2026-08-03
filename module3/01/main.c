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

  // for pipe
  int fd_p2c[2];
  int fd_c2p[2];

  // for fifo
  char fifo_c2p_name[256];

  if (fifo_name == NULL) { // pipe
    int res_p2c = pipe(fd_p2c);
    if (res_p2c == -1) {
      perror("pipe p2c");
      exit(EXIT_FAILURE);
    }
    int res_c2p = pipe(fd_c2p);
    if (res_c2p == -1) {
      perror("pipe c2p");
      exit(EXIT_FAILURE);
    }
  } else { // fifo
    snprintf(fifo_c2p_name, sizeof(fifo_c2p_name), "%s.ack", fifo_name);

    // pre-cleaning
    unlink(fifo_name);
    unlink(fifo_c2p_name);

    int res_fifo1 = mkfifo(fifo_name, 0666);
    if (res_fifo1 == -1) {
      perror("mkfifo main");
      exit(EXIT_FAILURE);
    }
    int res_fifo2 = mkfifo(fifo_c2p_name, 0666);
    if (res_fifo2 == -1) {
      perror("mkfifo ack");
      exit(EXIT_FAILURE);
    }
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    printf("[Child] Child process started\n");

    int read_fd;
    int write_fd;

    if (fifo_name == NULL) {
      // if pipe
      close(fd_p2c[1]);
      close(fd_c2p[0]);

      read_fd = fd_p2c[0];
      write_fd = fd_c2p[1];
    } else {
      // if fifo
      int open_write = open(fifo_c2p_name, O_WRONLY); // O_WRONLY === 1, write
      if (open_write == -1) {
        perror("Child open write FIFO");
        exit(EXIT_FAILURE);
      }
      write_fd = open_write;

      int open_read = open(fifo_name, O_RDONLY); // O_RDONLY === 0, read
      if (open_read == -1) {
        perror("Child open read FIFO");
        exit(EXIT_FAILURE);
      }
      read_fd = open_read;
    }
  } else {
    printf("[Parent] Parent created child process with PID = %d\n", pid);
    waitpid(pid, NULL, 0);
  }

  return 0;
}