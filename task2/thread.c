#include <stdio.h>
#include <stdlib.h>
#include "int_count.h"
#include "thread_opt.h"

const unsigned nSegment = 1000000000;

/*calculating ln(cos(x)+x^2) from 10 to 100)*/

int main(int argc, char* argv[])
{
    int nThreads;
    if (argc < 2)
    {
        printf ("enter number of threads\n");
        return 0;
    } 

    sscanf (argv[1], "%d", &nThreads);

    ThreadInfo info = {0,100,0,10000000,nThreads};

    CreateThreads (nThreads, NULL, &info);

    //printf ("%lg\n", SimpsonMethod (2,4,1000000000,sqr));
    return 0;
}