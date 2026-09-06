#include "sem_shm_list.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  srand(time(NULL) ^ getpid());

  int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
  if (fd == -1)
    err(EXIT_FAILURE, "shm_open (stale object? run make clean)");

  if (ftruncate(fd, MEM_SIZE) == -1)
    err(EXIT_FAILURE, "ftruncate");

  char *base = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (base == MAP_FAILED)
    err(EXIT_FAILURE, "mmap");

  close(fd);

  sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
  if (sem == SEM_FAILED)
    err(EXIT_FAILURE, "sem_open (stale object? run make clean)");

  SharedHeader *shdr = (SharedHeader *)base;
  shdr->producer_done = 0;

  printf("producer: shm=%s sem=%s\n", SHM_NAME, SEM_NAME);

  size_t cursor = sizeof(SharedHeader);
  int prev_offset = -1;

  while (1) {
    size_t remaining = MEM_SIZE - cursor;
    if (remaining <= sizeof(BlockHeader))
      break;

    int max_count = (remaining - sizeof(BlockHeader)) / sizeof(int);
    if (max_count < 1)
      break;

    int count = rand() % 100 + 1;
    if (count > max_count)
      count = max_count;

    sem_lock(sem);

    BlockHeader *block = (BlockHeader *)(base + cursor);
    block->count = count;
    block->next_offset = 0;

    for (int i = 0; i < count; i++) {
      block->data[i] = rand() % 200 - 100;
    }

    if (prev_offset != -1) {
      BlockHeader *prev = (BlockHeader *)(base + prev_offset);
      prev->next_offset = (int)cursor;
    }

    sem_unlock(sem);

    printf("producer: block at %zu, count=%d\n", cursor, count);

    prev_offset = (int)cursor;
    cursor += sizeof(BlockHeader) + count * sizeof(int);

    usleep((rand() % 500 + 100) * 1000);
  }

  sem_lock(sem);
  shdr->producer_done = 1;
  sem_unlock(sem);

  printf("producer: memory exhausted, done writing\n");

  if (prev_offset == -1) {
    printf("producer: no blocks written, nothing to wait for\n");
    exit(EXIT_SUCCESS);
  }

  while (1) {
    if (sem_lock(sem)) {
      printf("producer: resources already removed by consumer, exiting\n");
      break;
    }

    int all_processed = 1;
    size_t off = sizeof(SharedHeader);

    while (1) {
      BlockHeader *b = (BlockHeader *)(base + off);

      if (b->count != 0) {
        all_processed = 0;
        break;
      }
      if (b->next_offset == 0)
        break;
      off = (size_t)b->next_offset;
    }
    sem_unlock(sem);

    if (all_processed)
      break;
    usleep((rand() % 500 + 200) * 1000);
  }

  printf("producer: all sets processed, exiting (consumer will clean up)\n");

  munmap(base, MEM_SIZE);
  sem_close(sem);

  exit(EXIT_SUCCESS);
}
