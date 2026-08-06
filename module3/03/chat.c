#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_MSG_SIZE 256
#define MAX_MSGS 10
#define EXIT_PRIORITY 31

static char q1_name[128], q2_name[128];
static mqd_t mq_in, mq_out;
static int is_creator;
static volatile sig_atomic_t running = 1;

static void cleanup(void) {
  mq_close(mq_in);
  mq_close(mq_out);

  if (is_creator) {
    mq_unlink(q1_name);
    mq_unlink(q2_name);
    printf("[system] Queues deleted.\n");
  }
}

static void sigint_handler(int sig) {
  (void)sig;
  running = 0;
  const char bye[] = "BYE";
  mq_send(mq_out, bye, sizeof(bye), EXIT_PRIORITY);
  cleanup();
  _exit(0);
}

static void *recv_loop(void *arg) {
  (void)arg;
  char buf[MAX_MSG_SIZE];
  unsigned int prio;

  while (running) {
    ssize_t n = mq_receive(mq_in, buf, sizeof(buf), &prio);

    if (n < 0) {
      if (errno == EINTR)
        continue;
      break;
    }

    if (prio == EXIT_PRIORITY) {
      printf("\n[system] Peer disconnected.\n");
      running = 0;
      break;
    }

    buf[n] = '\0';
    printf("\n[peer]  %s\n[you]   ", buf);
    fflush(stdout);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <queue_name>\n", argv[0]);
    return 1;
  }

  snprintf(q1_name, sizeof(q1_name), "/%s_1", argv[1]);
  snprintf(q2_name, sizeof(q2_name), "/%s_2", argv[1]);

  struct mq_attr attr = {.mq_flags = 0,
                         .mq_maxmsg = MAX_MSGS,
                         .mq_msgsize = MAX_MSG_SIZE,
                         .mq_curmsgs = 0};

  mqd_t tmp = mq_open(q1_name, O_CREAT | O_EXCL | O_RDONLY, 0600, &attr);

  if (tmp != (mqd_t)-1) {
    is_creator = 1;
    mq_in = tmp;
    mq_out = mq_open(q2_name, O_CREAT | O_WRONLY, 0600, &attr);
    printf("[system] Queues created. Waiting for peer...\n");
  } else if (errno == EEXIST) {
    is_creator = 0;
    mq_out = mq_open(q1_name, O_WRONLY);
    mq_in = mq_open(q2_name, O_RDONLY);
    printf("[system] Connected to existing queues.\n");
  } else {
    perror("mq_open");
    return 1;
  }

  signal(SIGINT, sigint_handler);

  pthread_t tid;
  pthread_create(&tid, NULL, recv_loop, NULL);

  printf("[you]   ");
  fflush(stdout);

  char buf[MAX_MSG_SIZE];

  while (running && fgets(buf, sizeof(buf), stdin)) {
    buf[strcspn(buf, "\n")] = '\0';
    /* explanation of line 109:
    fgets includes '\n' from stdin, when user pressed Enter.
    Replace it with '\0' to avoid extra newline in the sent message. */
    if (!running)
      break;
    if (mq_send(mq_out, buf, strlen(buf) + 1, 1) < 0) {
      if (running)
        perror("mq_send");
      break;
    }
    printf("[you]   ");
    fflush(stdout);
  }

  running = 0;
  const char bye[] = "BYE";
  mq_send(mq_out, bye, sizeof(bye), EXIT_PRIORITY);
  pthread_join(tid, NULL);
  cleanup();
  return 0;
}