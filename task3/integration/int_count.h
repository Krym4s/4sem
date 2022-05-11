#ifndef INT_COUNT
#define INT_COUNT

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <unistd.h>
#include <stdbool.h>

#include <pthread.h>

#include <sys/sysinfo.h>
#include <sys/types.h>

#include <math.h>

double SimpsonMethod (double leftBorder, double rightBorder, unsigned nStep, double (*function)(double parameter));
double sqr (double parameter);
void OneThreadSimpson (void* resources);
void Integrate (void* resources);

#endif //INT_COUNT