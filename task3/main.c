#include "shared_net.h"
#include <stdlib.h>
#include <string.h>

InfoCommon ParseArgs (int args, char *argv[]);
InfoCommon ParseArgs2 (int argc, char *argv[]);
int * getNThreadsArray (int args, char *argv[]);

int main (int argc, char *argv[]) {

    InfoCommon start = ParseArgs2 (argc, argv);

    switch (start.server) {
        case -1:
            fprintf(stderr, "Error in parsing args.\n");
            return 0;

        case 1:
            RunClient(start.nComputers);
            break;
        
        case 2:
            RunServer(start.nThreads);
            break;

        default:
            fprintf(stderr, "Error in parsing args: [%d]\n", start.server);
            return -1;
        
    }


    return 0;
}

InfoCommon ParseArgs2 (int argc, char* argv[])
{
   InfoCommon infoCommon = {0,0,0};

   if (argc != 3)
   {
       infoCommon.server = -1;
       return infoCommon;
   } 
   if (*(argv[1]) == 'c')
    {
        int nComp;
        sscanf (argv[2], "%d", &nComp);
        infoCommon.nComputers = nComp;
        infoCommon.server = 1;

    }else if (*(argv[1]) == 's')
    {
        infoCommon.server = 2;
        int nThread;
        sscanf (argv[2], "%d", &nThread);
        infoCommon.nThreads = nThread;
    }else
    {
        infoCommon.server = -1;
        return infoCommon;
    }
    
    return infoCommon;
   

}

InfoCommon ParseArgs (int argc, char *argv[]) {
    InfoCommon infoCommon = {0,0,0};

    if (argc == 1)
    {
        infoCommon.server = -1;
        return infoCommon;
    }
        
    if (*(argv[1]) == 'c')
    {
        if (argc < 4)
        {
            infoCommon.server = -1;
            return infoCommon;
        }

        int nComp;
        sscanf (argv[2], "%d", &nComp);
        infoCommon.nComputers = nComp;
        infoCommon.nThreads = getNThreadsArray (argc, argv);
        if (infoCommon.nThreads == NULL)
        {
            infoCommon.server = -1;
            return infoCommon;
        }
        infoCommon.server = 1;

    }else if (*(argv[1]) == 's')
    {
        infoCommon.server = 2;
    }else
    {
        infoCommon.server = -1;
        return infoCommon;
    }
    
    return infoCommon;
}

int* getNThreadsArray (int argc, char *argv[])
{
    int nComp,arg;
    sscanf (argv[2], "%d", &nComp);

    if (nComp != argc - 3)
        return NULL;

    int* threadn = (int*) calloc (nComp, sizeof (int));
    if (threadn == NULL)
        return NULL;

    for (unsigned idx = 0; idx < nComp; idx++)
    {
        sscanf (argv[3 + idx], "%d", &arg);
        threadn[idx] = arg;
    }
    return threadn;
}


