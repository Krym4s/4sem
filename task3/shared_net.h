#ifndef SHARED_NET
#define SHARED_NET
#include <stdio.h>
#include <stdbool.h>

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

#define UDP_PORT 6666
#define TCP_PORT 8888

enum error {
    SUCCESS = 0,
    E_BADARGS,
    E_SOCKET,
    E_CONNECTION,
    E_MEMORY,
    E_THREAD,
};

typedef struct InfoCommon {
    int server;
    int* nThreads;
    size_t nComputers;
} InfoCommon;

typedef struct {
    int nThreads;
    int begThread;
} Task;

#define ERROR(condition, reason, err_code) do {if (condition) {printf ("Error:%s, line:%d\n",reason, __LINE__); return err_code;} }while(false);

int setKeepAlive (int socketID, int keepcnt, int keepidle, int keepintvl);
void RunServer(unsigned nthreads);
void RunClient(size_t nComputers);
#endif // SHARED_NET