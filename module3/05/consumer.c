#include "sem_shm_list.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  srand(time(NULL) ^ getpid());

  int fd = shm_open(SHM_NAME, O_RDWR, 0666);
  if (fd == -1)
    err(EXIT_FAILURE, "shm_open (is producer running?)");

  char *base = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (base == MAP_FAILED)
    err(EXIT_FAILURE, "mmap");

  close(fd);

  sem_t *sem = sem_open(SEM_NAME, 0);
  if (sem == SEM_FAILED)
    err(EXIT_FAILURE, "sem_open (is producer running?)");

  SharedHeader *shdr = (SharedHeader *)base;

  printf("consumer[%d]: attached, shm=%s sem=%s\n", getpid(), SHM_NAME,
         SEM_NAME);

  while (1) {
    if (sem_lock(sem)) {
      printf("consumer[%d]: resources already removed, exiting\n", getpid());
      break;
    }

    int found_unprocessed = 0;
    size_t off = sizeof(SharedHeader);

    while (1) {
      BlockHeader *b = (BlockHeader *)(base + off);

      if (b->count != 0) {
        int min = b->data[0], max = b->data[0];
        for (int i = 1; i < b->count; i++) {
          if (b->data[i] < min)
            min = b->data[i];
          if (b->data[i] > max)
            max = b->data[i];
        }
        printf("consumer[%d]: block at %zu, count=%d, min=%d, max=%d\n",
               getpid(), off, b->count, min, max);

        b->count = 0;
        found_unprocessed = 1;
        break;
      }

      if (b->next_offset == 0)
        break;
      off = (size_t)b->next_offset;
    }

    int should_cleanup = 0;
    if (!found_unprocessed && shdr->producer_done && !shdr->cleanup_done) {
      shdr->cleanup_done = 1;
      should_cleanup = 1;
    }

    sem_unlock(sem);

    if (should_cleanup) {
      printf("consumer[%d]: all sets processed, cleaning up\n", getpid());

      munmap(base, MEM_SIZE);

      shm_unlink(SHM_NAME);

      sem_close(sem);
      sem_unlink(SEM_NAME);
      break;
    }

    if (!found_unprocessed && !shdr->producer_done) {
      usleep((rand() % 500 + 200) * 1000);
      continue;
    }

    if (!found_unprocessed && shdr->cleanup_done) {
      printf(
          "consumer[%d]: cleanup already done by another consumer, exiting\n",
          getpid());
      munmap(base, MEM_SIZE);
      sem_close(sem);
      break;
    }

    usleep((rand() % 500 + 200) * 1000);
  }

  exit(EXIT_SUCCESS);
}
