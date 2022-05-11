#include "server.h"
#include "../shared_net.h"
#include "../integration/int_count.h"
#include "../integration/thread_opt.h"

struct sockaddr_in GetHelloMessage (int* error) {

    struct sockaddr_in peerAddr;
    socklen_t peerAddrLen = sizeof peerAddr;

    memset (&peerAddr, 0, sizeof peerAddr);

    struct sockaddr_in sockAddr;

    memset (&sockAddr, 0, sizeof sockAddr);

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons (UDP_PORT);
    sockAddr.sin_addr.s_addr = htonl (INADDR_ANY);

    int socketID = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketID == -1) {

        if (error)
            *error = E_SOCKET;

        return peerAddr;
    }

    if (bind (socketID, (struct sockaddr*) &sockAddr, sizeof sockAddr) == -1) {

        close (socketID);

        if (error)
            *error = E_SOCKET;

        return peerAddr;
    }

    int serverPortTCP = 0;

    for (int i = 0; serverPortTCP != TCP_PORT; i++){
        serverPortTCP = 0;

        recvfrom (socketID, &serverPortTCP, sizeof (int), 0,
                  (struct sockaddr*) &peerAddr, &peerAddrLen);

    
    }

    struct sockaddr_in res;

    memset (&res, 0, sizeof res);

    res.sin_family = AF_INET;
    res.sin_port = htons (TCP_PORT);
    res.sin_addr = peerAddr.sin_addr;

    if (error)
        *error = SUCCESS;

    return res;
}

int ConnectServer (struct sockaddr_in servAddr, int *const error) {

    int socketID = -1;
    struct sockaddr_in server_addr = servAddr;

    int err = SUCCESS;

    socketID = socket (AF_INET, SOCK_STREAM, 0);
    if (socketID < 0) 
        err = E_SOCKET;

    if (err == SUCCESS && (setKeepAlive(socketID, 2, 1, 1) != 0))
        err = E_SOCKET;

    if (err == SUCCESS && (connect(socketID, (struct sockaddr*) &server_addr, sizeof (server_addr)) < 0)){
        close (socketID);
        err = E_CONNECTION;
    }

    printf("[Successfuly connected to server]\n");
    if (error)
        *error = err;

    if (err != SUCCESS)
        return -1;

    return socketID;
}

int Compute(int socketID) {

    if (socketID < 0)
        return E_BADARGS;

    ssize_t ret;

    ThreadInfo infoInit;

    if (recv (socketID, &infoInit, sizeof infoInit, 0) != sizeof infoInit)
        return E_CONNECTION;

    double res = 0;

    ret = CreateThreads (infoInit.nProc, NULL, &infoInit);
    if (ret != SUCCESS)
        return ret;

    res = infoInit.result;

    if (send(socketID, &res, sizeof res, MSG_NOSIGNAL) != sizeof res)
        return E_CONNECTION;

    char ping = 0;

    if (send (socketID, &ping, sizeof ping, MSG_NOSIGNAL) != sizeof ping)
        return E_CONNECTION;

    return SUCCESS;
}
