#ifndef THREAD_OPT
#define THREAD_OPT

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <unistd.h>
#include <stdbool.h>

#include <pthread.h>

#include <sys/sysinfo.h>
#include <sys/types.h>

#include <math.h>

enum ThreadErrors
{
    NO_ERROR,
    NO_MEMORY,
    NO_THREADS,
    THREAD_FAULT
};

typedef struct
{
    double leftBorder;
    double rightBorder;

    double result;
    unsigned nStep;

    int nProc;
} ThreadInfo;

enum ThreadErrors CreateThreads (unsigned nThreads, void (*function)(void* resources), void* resources);
void* ThreadPrepare (void* resources, unsigned nThreads, unsigned nProc, unsigned effectiveSize);
void* ThreadStart (void* resources);


#endif //THREAD_OPT