#ifndef MAIN_H
#define MAIN_H

#define MAX_TEXT 512
#define MAX_TOPIC 64
#define MAX_SUBS 100
#define MAX_PUBS 100
#define PROJ_ID 'A'
#define FILE_PATH "/tmp"
#define SHUTDOWN_TIMEOUT 5 // время в секундах на очистку очереди после SIGINT

typedef struct {
  long mtype;
  char mtext[MAX_TEXT];
} MSGBUF;

typedef struct {
  int pid;
  char topic[MAX_TOPIC];
} Subscription;

void run_broker(void);
void run_publisher(const char *topic);
void run_subscriber(int argc, char *argv[]);

#endif