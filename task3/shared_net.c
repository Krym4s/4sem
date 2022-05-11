#include "shared_net.h"
#include "server/server.h"
#include "client/client.h"

int setKeepAlive (int socketID, int keepcnt, int keepidle, int keepintvl) {

    int trueF = 1;

    if (setsockopt (socketID, SOL_SOCKET, SO_KEEPALIVE, &trueF, sizeof true) != 0)
        ERROR (true, "setsockopt (timeout) for server socket", -1);

    if (setsockopt(socketID, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int)) != 0)
        ERROR (true, "tcp_keepcnt", -1);

    if (setsockopt (socketID, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int)) != 0)
        ERROR (true, "tcp_keepcnt", -1);

    if (setsockopt(socketID, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int)) != 0)
        ERROR (true, "tcp_keepcnt", -1);

    return 0;
}

const double leftBorder = 10.0;
const double rightBorder = 100.0;
const unsigned nStep = 80640000;

static void* SendHello (void* arg) {

    int error = SUCCESS;
    while (1) {
        error = SendHelloMessage();

        if (error != SUCCESS) {
            pthread_exit (NULL);
        }
    }
}

static void* GetTCPs (void* arg) {

    TasksWorkers* tasks = arg;

    int error = GetConnectionsTCP(tasks);

    if (error != SUCCESS) {
        fprintf (stderr, "Failed connection\n");
    }

    return NULL;
}

void RunClient(size_t nComputers, int* nThreads) {

    int error = SUCCESS;

    TasksWorkers* tasks;
    error = DevideWork(nComputers, nThreads, &tasks, leftBorder, rightBorder, nStep);
    if (error != SUCCESS) {
        fprintf (stderr, "Failed\n");
        return;
    }

    pthread_t thSender, thGetter;

    if (pthread_create(&thGetter, NULL, GetTCPs, tasks) != 0) {
        perror ("pthread_create getter:");
        return;
    }

    if (pthread_create(&thSender, NULL, SendHello, NULL) != 0) {
        perror ("pthread_create sender:");
        return;
    }

    if (pthread_join(thGetter, NULL) != 0) {
        perror ("pthread_join getter:");
        for (int i = 0; i < tasks->size; i++)
            close (tasks->task[i].socket);

        free (tasks);
        return;
    }

    pthread_cancel(thSender);

    double res = 0;
    if (GetResult(tasks, &res) != SUCCESS){
        fprintf (stderr, "Failed\n");
        for (int i = 0; i < tasks->size; i++)
            close (tasks->task[i].socket);

        free (tasks);
        return;
    }

    printf ("Result: %f\n", res);
    for (int i = 0; i < tasks->size; i++)
            close (tasks->task[i].socket);

    free (tasks);
    return;    
}

void RunServer() {

    int error = SUCCESS;

    struct sockaddr_in AddrServer = GetHelloMessage(&error);

    if (error != SUCCESS) {
        return;
    }

    int socketServer = ConnectServer(AddrServer, &error);

    if (error != SUCCESS) {
        close (socketServer);
        return;
    }

    error = Compute(socketServer);

    if (error != SUCCESS) {
        close (socketServer);
        return;
    }

    printf ("[SUCCESS:Finished]\n");

    close (socketServer);
}
