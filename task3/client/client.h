#ifndef CLIENT_INT
#define CLIENT_INT

#define _GNU_SOURCE
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/sysinfo.h>
#include <sys/select.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include "../shared_net.h"
#include "../integration/int_count.h"
#include "../integration/thread_opt.h"

typedef struct {
    double begin;
    double end;
    size_t nThreads;
} InfoWorker;

typedef struct {
    ThreadInfo infoWorker;
    int socket;
} InfoHandler;

typedef struct {
    InfoHandler* task;
    size_t size;
} TasksWorkers;

int DevideWork (unsigned nComp, TasksWorkers** tasks, double leftBorder, double rightBorder, double nStep);
int CreateSocket ();
int GetConnectionsTCP (TasksWorkers* tasks);
int SendHelloMessage ();
void* HandlerWork (void* arg);
int GetResult (TasksWorkers *tasks, double *res, int sum);

#endif //CLIENT_INT