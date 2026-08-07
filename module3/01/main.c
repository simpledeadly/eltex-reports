#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct FileHeader {
  char filename[256];
  long filesize;
};

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

    while (1) {
      if (write(write_fd, "R", 1) < 0) {
        perror("write ACK");
        break;
      }

      struct FileHeader header;
      if (read(read_fd, &header, sizeof(header)) <= 0 ||
          header.filesize == -1) {
        // n == -1: read error
        // n == 0; parent closed pipe, eof
        break;
      }

      char out_name[300];
      snprintf(out_name, sizeof(out_name), "%s.copy", header.filename);

      int out_fd = open(out_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
      if (out_fd == -1) {
        perror("Child open out_fd");
        exit(EXIT_FAILURE);
      }

      char buffer[512];
      long remaining = header.filesize;

      while (remaining > 0) {
        long to_read = sizeof(buffer);
        if (remaining < to_read) {
          to_read = remaining;
        }
        ssize_t bytes_read = read(read_fd, buffer, to_read);
        if (bytes_read <= 0) {
          break;
        }
        ssize_t bytes_written = write(out_fd, buffer, bytes_read);
        if (bytes_written < 0) {
          perror("[Child] write to file");
          break;
        }
        remaining -= bytes_read;
      }
      close(out_fd);
      printf("[Child] Saved file: %s (%ld bytes)\n", out_name, header.filesize);
    }

    close(read_fd);
    close(write_fd);
    printf("[Child] All files received. Exiting.\n");
    exit(EXIT_SUCCESS);
  } else {
    printf("[Parent] Parent created child process with PID = %d\n", pid);

    int read_fd;
    int write_fd;

    if (fifo_name == NULL) {
      // if pipe
      close(fd_p2c[0]);
      close(fd_c2p[1]);

      read_fd = fd_c2p[0];
      write_fd = fd_p2c[1];
    } else {
      // if fifo
      int open_read = open(fifo_c2p_name, O_RDONLY); // O_RDONLY === 0, read
      if (open_read == -1) {
        perror("Parent open read FIFO");
        exit(EXIT_FAILURE);
      }
      read_fd = open_read;

      int open_write = open(fifo_name, O_WRONLY); // O_WRONLY === 1, write
      if (open_write == -1) {
        perror("Parent open write FIFO");
        exit(EXIT_FAILURE);
      }
      write_fd = open_write;
    }

    for (int i = optind; i < argc; i++) {
      const char *filepath = argv[i];

      struct stat st;
      int res_stat = stat(filepath, &st);
      if (res_stat == -1) {
        fprintf(stderr, "[Parent] Error: File '%s' not found.\n", filepath);
        continue;
      }

      char ack;
      read(read_fd, &ack, 1);

      struct FileHeader header;
      memset(&header, 0, sizeof(header));
      strncpy(header.filename, filepath, sizeof(header.filename) - 1);
      header.filesize = st.st_size;

      if (write(write_fd, &header, sizeof(header)) < 0) {
        perror("[Parent] write header");
        break;
      }

      int in_fd = open(filepath, O_RDONLY);
      if (in_fd == -1) {
        perror("[Parent] open source file");
        continue;
      }
      char buffer[512];
      ssize_t bytes_read;
      while ((bytes_read = read(in_fd, buffer, sizeof(buffer))) > 0) {
        if (write(write_fd, buffer, bytes_read) < 0) {
          perror("[Parent] write data");
          break;
        }
      }
      close(in_fd);
      printf("[Parent] Sent file: %s (%ld bytes)\n", filepath,
             (long)st.st_size);
    }

    char ack;
    struct FileHeader stop_header;
    memset(&stop_header, 0, sizeof(stop_header));
    stop_header.filesize = -1;

    if (read(read_fd, &ack, 1) <= 0) {
      fprintf(stderr, "[Parent] Failed to read final ACK.\n");
    } else {
      if (write(write_fd, &stop_header, sizeof(stop_header)) < 0) {
        perror("[Parent] write stop header");
      }
    }

    close(read_fd);
    close(write_fd);

    waitpid(pid, NULL, 0);

    if (fifo_name != NULL) {
      unlink(fifo_name);
      unlink(fifo_c2p_name);
    }
    printf("[Parent] All files transferred. Child finished.\n");
  }

  return 0;
}