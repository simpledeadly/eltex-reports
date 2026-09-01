#include "sem_shm_list.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  srand(time(NULL) ^ getpid());

  key_t key = get_ipc_key();

  int shmid = shmget(key, MEM_SIZE, IPC_CREAT | 0666);
  if (shmid == -1) {
    err(EXIT_FAILURE, "shmget");
  }

  int semid = semget(key, 1, IPC_CREAT | 0666);
  if (semid == -1)
    err(EXIT_FAILURE, "semget");

  union semun arg;
  arg.val = 1;
  if (semctl(semid, 0, SETVAL, arg) == -1)
    err(EXIT_FAILURE, "semctl SETVAL");

  char *base = shmat(shmid, NULL, 0);
  if (base == (void *)-1)
    err(EXIT_FAILURE, "shmat");

  SharedHeader *shdr = (SharedHeader *)base;

  shdr->producer_done = 0;

  printf("producer: shmid=%d semid=%d\n", shmid, semid);

  size_t cursor = sizeof(SharedHeader);
  int prev_offset = -1; // no written blocks yet

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

    sem_lock(semid);

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

    sem_unlock(semid);

    printf("producer: block at %zu, count=%d\n", cursor, count);

    prev_offset = (int)cursor;
    cursor += sizeof(BlockHeader) + count * sizeof(int);

    usleep((rand() % 500 + 100) * 1000);
  }

  sem_lock(semid);
  shdr->producer_done = 1;
  sem_unlock(semid);
  printf("producer: memory exhausted, done writing\n");

  if (prev_offset == -1) {
    printf("producer: no blocks written, nothing to wait for\n");
    exit(EXIT_SUCCESS);
  }

  while (1) {
    if (sem_lock(semid)) {
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
    sem_unlock(semid);

    if (all_processed)
      break;
    usleep((rand() % 500 + 200) * 1000);
  }

  printf("producer: all sets processed, exiting (consumer will clean up)\n");
  exit(EXIT_SUCCESS);
}
