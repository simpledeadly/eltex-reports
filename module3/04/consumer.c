#include "sem_shm_list.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  srand(time(NULL) ^ getpid());

  key_t key = get_ipc_key();

  int shmid = shmget(key, MEM_SIZE, 0666);
  if (shmid == -1)
    err(EXIT_FAILURE, "shmget (is producer running?)");

  int semid = semget(key, 1, 0666);
  if (semid == -1)
    err(EXIT_FAILURE, "semget (is producer running?");

  char *base = shmat(shmid, NULL, 0);
  if (base == (void *)-1)
    err(EXIT_FAILURE, "shmat");

  SharedHeader *shdr = (SharedHeader *)base;

  printf("consumer[%d]: attached, shmid=%d semid=%d\n", getpid(), shmid, semid);

  while (1) {
    if (sem_lock(semid)) {
      printf("consumer[%d]: resources already removed, exiting\n", getpid());
      break;
    }

    int found_unprocessed = 0;
    int all_processed = 1;
    size_t off = sizeof(SharedHeader);

    while (1) {
      BlockHeader *b = (BlockHeader *)(base + off);

      if (b->count != 0) {
        all_processed = 0;

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
    if (!found_unprocessed && all_processed && shdr->producer_done &&
        !shdr->cleanup_done) {
      shdr->cleanup_done = 1;
      should_cleanup = 1;
    }

    sem_unlock(semid);

    if (should_cleanup) {
      printf("consumer[%d]: all sets processed, cleaning up\n", getpid());
      if (shmdt(base) == -1)
        err(EXIT_FAILURE, "shmdt");
      if (shmctl(shmid, IPC_RMID, NULL) == -1)
        err(EXIT_FAILURE, "shmctl IPC_RMID");
      union semun dummy;
      if (semctl(semid, 0, IPC_RMID, dummy) == -1)
        err(EXIT_FAILURE, "semctl IPC_RMID");
      break;
    }

    if (!found_unprocessed && !all_processed) {
      fprintf(stderr, "consumer[%d]: warning: inconsistent state\n", getpid());
    }

    if (!found_unprocessed && all_processed && !shdr->producer_done) {
      usleep((rand() % 500 + 200) * 1000);
      continue;
    }

    if (!found_unprocessed && all_processed && shdr->cleanup_done) {
      printf(
          "consumer[%d]: cleanup already done by another consumer, exiting\n",
          getpid());
      break;
    }

    usleep((rand() % 500 + 200) * 1000);
  }

  exit(EXIT_SUCCESS);
}
