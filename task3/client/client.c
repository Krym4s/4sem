#include "client.h"

/*
 * Function that creates socket for connection with servers
 * it initiates socket and sets it options
 */

#define end_without_closing do {close (serverSocket);  return error;} while(false);

int CreateSocket ()
{
    struct sockaddr_in servAddr;

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1)
        ERROR (true, "Socket was not created", -1);

    bool setsockres = true;

    if (setsockopt (socketFd, SOL_SOCKET, SO_REUSEADDR, &setsockres, sizeof true) != 0)
        ERROR (true, "Socket option for binding addres was not setted", -1);

    struct timeval timeout = {
            .tv_sec = 10,
            .tv_usec = 0
    };

    if (setsockopt (socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) != 0)
        ERROR (true, "Socket option for timeout was not setted", -1);

    if (setKeepAlive (socketFd, 4, 3, 1) != 0)
        ERROR (true, "Socket alive check was not setted",-1);

    memset (&servAddr, 0, sizeof servAddr);

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servAddr.sin_port = htons (TCP_PORT);

    if (bind (socketFd, (struct sockaddr*) &servAddr, sizeof servAddr) < 0) 
        ERROR (true, "Binding server socket", -1);

    return socketFd;
}

int GetConnectionsTCP (TasksWorkers* tasks) {

    if (tasks == NULL)
        return E_BADARGS;

    int error = SUCCESS;

    int serverSocket;
    int newSocket;

    struct sockaddr_in newAddr;
    socklen_t newAddrLen = sizeof newAddr;

    serverSocket = CreateSocket ();

    if (serverSocket == -1)
        end_without_closing;
    
    if (listen (serverSocket, 10) < 0) {
        error = E_SOCKET;
        end_without_closing;
    }

    int nConnectedSockets = 0;
    for (; nConnectedSockets < tasks->size; nConnectedSockets++) {

        newSocket = accept (serverSocket, (struct sockaddr*) &newAddr, &newAddrLen);
        if (newSocket < 0) {
            error = E_CONNECTION;
            for (int j = 0; j < nConnectedSockets; ++j)
            close (tasks->task[j].socket);
            close (serverSocket);
            return error;
        }

        tasks->task[nConnectedSockets].socket = newSocket;
        printf("[Success: Server is connected]\n");
    }


    end_without_closing;   
}

int DevideWork (unsigned nComp, TasksWorkers** task, double leftBorder, double rightBorder, double nStep)
{
    if (nComp == 0)
        return E_BADARGS;

    double compLeftBorder = leftBorder;
    double compStep = (rightBorder - leftBorder) / nComp;
    int curNStep = nStep;

    *task = (TasksWorkers*) malloc (sizeof (**task));

    if (*task == NULL)
        return E_MEMORY;

    (*task)->task = (InfoHandler*) malloc (sizeof (InfoHandler) * nComp);
    (*task)->size = nComp;

    if ((*task)->task == NULL)
    {
        free (*task);
        return E_MEMORY;
    }

    ThreadInfo* info;

    for (unsigned idx = 0; idx < nComp; idx++)
    {
        info = &(((*task)->task + idx)->infoWorker);
        info->leftBorder = compLeftBorder;
        info->rightBorder = compLeftBorder + compStep;
        info->nStep = curNStep;

        leftBorder += compStep;
    }

    return SUCCESS;
}

int SendHelloMessage () {

    struct sockaddr_in bc_addr;

    int socketID = socket (AF_INET, SOCK_DGRAM, 0);
    if (socketID == -1)
        return E_SOCKET;

    int trueF = 1;
    if (setsockopt (socketID, SOL_SOCKET, SO_BROADCAST, &trueF, sizeof true) < 0) {
        close (socketID);
        return E_SOCKET;
    }

    memset (&bc_addr, 0, sizeof bc_addr);

    bc_addr.sin_family = AF_INET;
    bc_addr.sin_port = htons(UDP_PORT);
    bc_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    int msg = TCP_PORT;
    if (sendto (socketID, &msg, sizeof msg, 0, (struct sockaddr *) &bc_addr, sizeof bc_addr) < 0) {
        close (socketID);
        return E_SOCKET;
    }

    close (socketID);
    return SUCCESS;
}

void* HandlerWork (void* arg)
{
    double* result = (double*) calloc (1, sizeof (double));

    if (result == NULL) 
        ERROR (true, "NO memory",result);

    InfoHandler* infoHandler = arg;

    if (send (infoHandler->socket, 
              &infoHandler->infoWorker, 
              sizeof infoHandler->infoWorker,
               0) != sizeof infoHandler->infoWorker)
    {
        free (result);
        ERROR (true, "impossible to send message to client", NULL);
    }

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(infoHandler->socket, &fdset);

    if (select (infoHandler->socket + 1, &fdset, NULL, NULL, NULL) != 1)
    {
        free (result);
        ERROR (true, "file descriptor was not seted on listening", NULL);
    }
    
    if (recv (infoHandler->socket, result, sizeof (double), 0) != sizeof (double))
    {
        free (result);
        ERROR (true, "answer was not recieved", NULL);
    }

    return result;
}

int GetResult (TasksWorkers *tasks, double *res, int sum) {

    if (!tasks || !res)
        return E_BADARGS;

    int error = SUCCESS;

    pthread_t* threadIDs = (pthread_t*) calloc (tasks->size, sizeof (pthread_t));

    ERROR (!threadIDs, "no memory", E_MEMORY);

    for (unsigned idx = 0; idx < tasks->size; idx++) {

        if (pthread_create (&threadIDs[idx], NULL, HandlerWork, (void *) &tasks->task[idx]) != 0)
        {
            for (int j = 0; j < idx; ++j)
                pthread_cancel (threadIDs[j]);

            free (threadIDs);
            ERROR (true, "pthread_create HandlerWork:",E_THREAD);
        }
    }

    double result = 0;
    double *part = NULL;

    for (int j = 0; j < tasks->size; ++j) {

        int check = pthread_join (threadIDs[j], (void **) &part);

        if (check != 0 || !part) {

            if (check != 0)
                fprintf (stderr, "Output of pthread_join() = %d\n", check);
            
            for (; j < tasks->size; ++j)
                pthread_cancel (threadIDs[j]);

            free (threadIDs);
            ERROR (true, "pthread_create HandlerWork:",E_THREAD);
        }

        result += *part;
        free (part);
    }

    *res = result;

    free (threadIDs);
    return error;
}
