#include "clent.h"

/*
 * Function that creates socket for connection with servers
 * it initiates socket and sets it options
 */

int CreateSocket ()
{
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1)
        ERROR (true, "socket was not created", -1);

    if (setsockopt (socketFd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof true) != 0)
        ERROR (true, "socket option for binding addres was not setted", -1);

    struct timeval timeout = {
            .tv_sec = 10,
            .tv_usec = 0
    };

    if (setsockopt (socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) != 0)
        ERROR (true, "socket option for timeout was not setted", -1);

    

        
}