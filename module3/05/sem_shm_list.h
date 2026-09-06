#ifndef SEM_SHM_LIST_H
#define SEM_SHM_LIST_H

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>

#define SHM_NAME "/m3t5_shm"
#define SEM_NAME "/m3t5_sem"
#define MEM_SIZE 4096

typedef struct {
  int producer_done;
  int cleanup_done;
} SharedHeader;

typedef struct {
  int count;
  int next_offset;
  int data[];
} BlockHeader;

static inline int sem_lock(sem_t *sem) {
  while (sem_wait(sem) == -1) {
    if (errno == EINTR)
      continue;
    if (errno == EINVAL)
      return 1;
    err(EXIT_FAILURE, "sem_wait");
  }
  return 0;
}

static inline int sem_unlock(sem_t *sem) {
  if (sem_post(sem) == -1) {
    if (errno == EINVAL)
      return 1;
    err(EXIT_FAILURE, "sem_post");
  }
  return 0;
}

#endif
