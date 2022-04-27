#include "thread_opt.h"
#include "int_count.h"

#define ERROR(condition, reason, err_code) do {if (condition) {printf ("Error:%s, line:%d\n",reason, __LINE__); return err_code;} }while(false);

enum ThreadErrors CreateThreads (unsigned nThreads, void (*function)(void* resources), void* resources)
{
    ERROR (nThreads == 0, "No threads", NO_THREADS);
        
    unsigned nProc = sysconf(_SC_NPROCESSORS_ONLN);

    unsigned curThreads = nProc < nThreads ? nThreads : nProc;
    pthread_t* threads = (pthread_t*) calloc (curThreads, sizeof(*threads));
    ERROR (threads == NULL, "No memory",NO_MEMORY);

    /*there is no reason to launch threads more than CPUs in this task*/

    unsigned cacheLineSize = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    unsigned effectiveSize = (sizeof(ThreadInfo)/cacheLineSize + 1) * cacheLineSize;

    void* threadInfo = ThreadPrepare (resources, nThreads, nProc, effectiveSize);
    ERROR (threadInfo == NULL, "No memory",NO_MEMORY);

    
    for (unsigned idx = 0; idx < curThreads; idx++)
        if (pthread_create (threads + idx, NULL, ThreadStart, threadInfo + idx * effectiveSize) != 0)
        {
            ERROR (true, "Thread not created", THREAD_FAULT)
        }

    double sum = 0;

    

    for (unsigned idx = 0; idx < curThreads; idx++)
    {
        if (pthread_join(threads[idx], NULL) != 0)
            ERROR(true,"pthread_join non-empty Threads",THREAD_FAULT);

        if (idx < nThreads)
            sum += ((ThreadInfo*)(threadInfo + idx * effectiveSize))->result;
    
    }

    printf ("%lf\n", sum);

    free (threadInfo);
    free (threads);

    return NO_ERROR;
}

void* ThreadPrepare (void* resources, unsigned nThreads, unsigned nProc, unsigned effectiveSize)
{
    /*get information about integrable segment*/

    double leftBorder = ((ThreadInfo*)resources)->leftBorder;
    double rightBorder = ((ThreadInfo*)resources)->rightBorder;
    double nStep = ((ThreadInfo*)resources)->nStep;
    double step = (rightBorder - leftBorder)/nProc;

    /*ThreadInfo's size equals number of CPUs*/

    unsigned realNThreads = nThreads < nProc ? nThreads : nProc;
    unsigned allThreads = nThreads;

    void* threadInfos;

    if (allThreads > realNThreads)  
        threadInfos = malloc (effectiveSize * allThreads); 
    else
        threadInfos = malloc (effectiveSize * nProc);

    if (threadInfos == NULL)
        return NULL;
    
    for (unsigned idx = 0; idx < realNThreads; idx++)
    {
        ThreadInfo* current = (ThreadInfo*)(threadInfos + idx * effectiveSize);
        current->leftBorder = leftBorder + step * idx;
        current->rightBorder = current->leftBorder + step;
        current->nProc = idx;
        current->nStep = nStep/realNThreads;
    }

    /*Not used threads*/

    for (unsigned idx = realNThreads; idx < nProc; idx++)
    {
        ThreadInfo* current = (ThreadInfo*)(threadInfos + idx * effectiveSize);
        current->leftBorder = leftBorder;
        current->rightBorder = leftBorder + step;
        current->nProc = idx;
        current->nStep = nStep/realNThreads;
    }

    for (unsigned idx = nProc; idx < allThreads; idx++)
    {
        ThreadInfo* current = (ThreadInfo*)(threadInfos + idx * effectiveSize);
        current->nProc = -1;
    }

    return threadInfos;
}

void* ThreadStart (void* resources)
{
    ThreadInfo* threadInfo = (ThreadInfo*) resources;
    int numCPU = threadInfo->nProc;

    if (numCPU != -1)
    {
        cpu_set_t cpu = {};
        pthread_t id = pthread_self();

        CPU_ZERO(&cpu);
        CPU_SET(numCPU, &cpu);

        if (pthread_setaffinity_np(id, sizeof(cpu_set_t), &cpu) < 0)
            ERROR (true, "Afinity_error",NULL)

        Integrate (resources);
    }

    
    return NULL;
}
