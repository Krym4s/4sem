#ifndef INT_COUNT
#define INT_COUNT

#include <math.h>

double SimpsonMethod (double leftBorder, double rightBorder, unsigned nStep, double (*function)(double parameter));
double sqr (double parameter);

#endif //INT_COUNT