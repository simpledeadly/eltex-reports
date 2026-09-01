#ifndef SEM_SHM_LIST_H
#define SEM_SHM_LIST_H

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

#define MEM_SIZE 4096
#define KEY_FILE_PATH "/tmp/m3t4_key"
#define PROJ_ID 'A'

typedef struct {
  int producer_done; // 0 - writing, 1 - finished
  int cleanup_done;
} SharedHeader;

typedef struct {
  int count;
  int next_offset;
  int data[]; // flexible array member
} BlockHeader;

static inline key_t get_ipc_key(void) {
  int fd = open(KEY_FILE_PATH, O_CREAT, 0600);
  if (fd == -1)
    err(EXIT_FAILURE, "open key file");
  close(fd);
  key_t key = ftok(KEY_FILE_PATH, PROJ_ID);
  if (key == -1)
    err(EXIT_FAILURE, "ftok");
  return key;
}

static inline int sem_op_safe(int semid, short op) {
  struct sembuf sop = {.sem_num = 0, .sem_op = op, .sem_flg = 0};
  if (semop(semid, &sop, 1) == -1) {
    if (errno == EINVAL || errno == EIDRM)
      return 1;
    err(EXIT_FAILURE, "semop");
  }
  return 0;
}

static inline int sem_lock(int semid) { return sem_op_safe(semid, -1); }
static inline int sem_unlock(int semid) { return sem_op_safe(semid, 1); }

#endif
