#include "main.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Глобальное состояние брокера
static int g_qid = -1;
static Subscription g_subs[MAX_SUBS];
static int g_subs_count = 0;
static int g_pubs[MAX_PUBS];
static int g_pubs_count = 0;
static volatile sig_atomic_t g_broker_shutdown = 0;

// Глобальное состояние издателя
static volatile sig_atomic_t g_pub_running = 1;

// Глобальное состояние подписчика
static int g_sub_qid = -1;
static int g_sub_pid = 0;
static int g_sub_argc = 0;
static char **g_sub_argv = NULL;
static volatile sig_atomic_t g_sub_running = 1;

/*
──────────────────────────────────────────────────────────────
Обработчики сигналов
──────────────────────────────────────────────────────────────
*/

// Обработчик SIGINT для брокера
static void broker_sigint(int sig) {
  (void)sig;
  g_broker_shutdown = 1;
}

// Обработчик SIGINT для издателя
static void pub_sigint(int sig) {
  (void)sig;
  g_pub_running = 0;
}

// Обработчик SIGINT для подписчика
static void sub_sigint(int sig) {
  (void)sig;
  if (g_sub_qid != -1) {
    MSGBUF msg;
    msg.mtype = 1;
    for (int i = 2; i < g_sub_argc; i++) {
      snprintf(msg.mtext, MAX_TEXT, "unsubscribe,%d,%s", g_sub_pid,
               g_sub_argv[i]);
      msgsnd(g_sub_qid, &msg, strlen(msg.mtext) + 1, IPC_NOWAIT);
    }
  }
  g_sub_running = 0;
}

// ───────────────────────────────────────────────────────────

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s -b | -p <topic> | -s <topic1> [topic2 ...]\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "-b") == 0) {
    run_broker();
  } else if (strcmp(argv[1], "-p") == 0) {
    if (argc != 3) {
      fprintf(stderr, "Usage: %s -p <topic>\n", argv[0]);
      return EXIT_FAILURE;
    }
    run_publisher(argv[2]);
  } else if (strcmp(argv[1], "-s") == 0) {
    if (argc < 3) {
      fprintf(stderr, "Usage: %s -s <topic1> [topic2 ...]\n", argv[0]);
      return EXIT_FAILURE;
    }
    run_subscriber(argc, argv);
  } else {
    fprintf(stderr, "Unknown mode: %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/*
──────────────────────────────────────────────────────────────
Брокер
──────────────────────────────────────────────────────────────
*/

void run_broker(void) {
  printf("Broker started (PID: %d).\n", getpid());

  key_t key = ftok(FILE_PATH, PROJ_ID);
  if (key == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  g_qid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
  if (g_qid == -1) {
    if (errno == EEXIST)
      fprintf(stderr, "Another broker is already running.\n");
    else
      perror("msgget");
    exit(EXIT_FAILURE);
  }
  printf("Queue created, id=%d\n", g_qid);

  // Устанавливаем обработчик SIGINT
  struct sigaction sa = {0};
  sa.sa_handler = broker_sigint;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);

  MSGBUF msg;

  // Основной цикл приёма сообщений
  while (!g_broker_shutdown) {
    /*
     * IPC_NOWAIT: не блокироваться, если сообщений нет.
     * Это позволяет регулярно проверять флаг g_broker_shutdown.
     * Если очередь пуста — ждём 100 мс и проверяем снова.
     */
    ssize_t r = msgrcv(g_qid, &msg, sizeof(msg.mtext), 1, IPC_NOWAIT);
    if (r == -1) {
      if (errno == ENOMSG) {
        // Очередь пуста, проверим флаг и подождём
        struct timespec ts = {0, 100000000L}; // 100 ms
        nanosleep(&ts, NULL);
        continue;
      }
      if (errno != EINTR)
        perror("msgrcv");
      break;
    }

    char *payload = "";
    char *newline = strchr(msg.mtext, '\n');
    if (newline) {
      *newline = '\0';
      payload = newline + 1;
    }

    char action[32], topic[MAX_TOPIC];
    int pid;

    if (sscanf(msg.mtext, "%31[^,],%d,%63s", action, &pid, topic) != 3) {
      fprintf(stderr, "Broker: bad message format: %s\n", msg.mtext);
      continue;
    }

    // subscribe
    if (strcmp(action, "subscribe") == 0) {
      if (g_subs_count < MAX_SUBS) {
        g_subs[g_subs_count].pid = pid;
        strncpy(g_subs[g_subs_count].topic, topic, MAX_TOPIC - 1);
        g_subs_count++;
        printf("Broker: %d subscribed to '%s'\n", pid, topic);
      }

      // unsubscribe
    } else if (strcmp(action, "unsubscribe") == 0) {
      for (int i = 0; i < g_subs_count; i++) {
        if (g_subs[i].pid == pid && strcmp(g_subs[i].topic, topic) == 0) {
          g_subs[i] = g_subs[g_subs_count - 1];
          g_subs_count--;
          printf("Broker: %d unsubscribed from '%s'\n", pid, topic);
          break;
        }
      }

      // send
    } else if (strcmp(action, "send") == 0) {
      int known = 0;
      for (int i = 0; i < g_pubs_count; i++)
        if (g_pubs[i] == pid) {
          known = 1;
          break;
        }
      if (!known && g_pubs_count < MAX_PUBS)
        g_pubs[g_pubs_count++] = pid;

      int delivered = 0;
      for (int i = 0; i < g_subs_count; i++) {
        if (strcmp(g_subs[i].topic, topic) == 0) {
          MSGBUF out;
          out.mtype = g_subs[i].pid;
          snprintf(out.mtext, MAX_TEXT, "[%s] %s", topic, payload);
          if (msgsnd(g_qid, &out, strlen(out.mtext) + 1, 0) == -1)
            perror("Broker: msgsnd to subscriber");
          else
            delivered++;
        }
      }
      printf("Broker: '%s' -> %d subscriber(s)\n", topic, delivered);
    }
  }

  // Завершение: уведомляем всех и чистим очередь
  printf("\nBroker: shutting down, notifying %d pub(s) and %d sub(s)...\n",
         g_pubs_count, g_subs_count);

  // Отправляем SIGINT всем известным процессам
  for (int i = 0; i < g_pubs_count; i++)
    kill(g_pubs[i], SIGINT);
  for (int i = 0; i < g_subs_count; i++)
    kill(g_subs[i].pid, SIGINT);

  /*
   * Ждём, пока подписчики успеют прислать unsubscribe
   * или истечёт таймаут SHUTDOWN_TIMEOUT секунд.
   */
  time_t deadline = time(NULL) + SHUTDOWN_TIMEOUT;
  while (g_subs_count > 0 && time(NULL) < deadline) {
    ssize_t r = msgrcv(g_qid, &msg, sizeof(msg.mtext), 1, IPC_NOWAIT);
    if (r > 0) {
      char action[32], topic[MAX_TOPIC];
      int pid;
      if (sscanf(msg.mtext, "%31[^,],%d,%63s", action, &pid, topic) == 3 &&
          strcmp(action, "unsubscribe") == 0) {
        for (int i = 0; i < g_subs_count; i++) {
          if (g_subs[i].pid == pid && strcmp(g_subs[i].topic, topic) == 0) {
            g_subs[i] = g_subs[g_subs_count - 1];
            g_subs_count--;
            break;
          }
        }
      }
    } else {
      struct timespec ts = {0, 50000000L}; // 50 ms
      nanosleep(&ts, NULL);
    }
  }
  if (g_subs_count > 0)
    printf("Broker: timeout, %d subscriber(s) did not unsubscribe.\n",
           g_subs_count);

  if (msgctl(g_qid, IPC_RMID, NULL) == -1)
    perror("msgctl IPC_RMID");
  printf("Broker: queue removed. Done.\n");
}

/*
──────────────────────────────────────────────────────────────
Издатель
──────────────────────────────────────────────────────────────
*/

void run_publisher(const char *topic) {
  printf("Publisher started: topic='%s' PID=%d\n", topic, getpid());

  key_t key = ftok(FILE_PATH, PROJ_ID);
  int qid = msgget(key, 0);
  if (qid == -1) {
    perror("msgget (is broker running?)");
    exit(EXIT_FAILURE);
  }

  struct sigaction sa = {0};
  sa.sa_handler = pub_sigint;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);

  MSGBUF msg;
  msg.mtype = 1;
  char payload[256];

  while (g_pub_running) {
    printf("Enter message (Ctrl+C or 'exit' to quit): ");
    fflush(stdout);

    if (fgets(payload, sizeof(payload), stdin) == NULL) {
      break;
    }
    payload[strcspn(payload, "\n")] = '\0';

    if (strcmp(payload, "exit") == 0)
      break;

    snprintf(msg.mtext, MAX_TEXT, "send,%d,%s\n%s", getpid(), topic, payload);

    if (msgsnd(qid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
      if (errno != EINTR)
        perror("msgsnd");
      break;
    }
  }
  printf("\nPublisher: done.\n");
}

/*
──────────────────────────────────────────────────────────────
Подписчик
──────────────────────────────────────────────────────────────
*/

void run_subscriber(int argc, char *argv[]) {
  g_sub_pid = getpid();
  g_sub_argc = argc;
  g_sub_argv = argv;

  printf("Subscriber started (PID: %d)\n", g_sub_pid);

  key_t key = ftok(FILE_PATH, PROJ_ID);
  g_sub_qid = msgget(key, 0);
  if (g_sub_qid == -1) {
    perror("msgget (is broker running?)");
    exit(EXIT_FAILURE);
  }

  struct sigaction sa = {0};
  sa.sa_handler = sub_sigint;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);

  // Подписываемся на все темы из аргументов (argv[2..])
  MSGBUF msg;
  for (int i = 2; i < argc; i++) {
    msg.mtype = 1;
    snprintf(msg.mtext, MAX_TEXT, "subscribe,%d,%s", g_sub_pid, argv[i]);
    if (msgsnd(g_sub_qid, &msg, strlen(msg.mtext) + 1, 0) == -1)
      perror("msgsnd subscribe");
    else
      printf("Subscribed to topic: '%s'\n", argv[i]);
  }

  while (g_sub_running) {
    ssize_t r =
        msgrcv(g_sub_qid, &msg, sizeof(msg.mtext), g_sub_pid, IPC_NOWAIT);
    if (r == -1) {
      if (errno == ENOMSG) {
        struct timespec ts = {0, 100000000L};
        nanosleep(&ts, NULL);
        continue;
      }
      if (errno != EINTR)
        perror("msgrcv");
      break;
    }
    printf("\n——— Message ———\n%s\n———————————————\n", msg.mtext);
  }

  printf("Subscriber %d: done.\n", g_sub_pid);
}
