#include "int_count.h"

double cos (double);
double log (double);

typedef struct 
{
    double leftBorder;
    double rightBorder;
    double result;
    unsigned nStep;
} IntInfo;

double SimpsonMethod (double leftBorder, double rightBorder, unsigned nStep, double (*function)(double parameter))
{
    double result = 0;
    double step = (rightBorder - leftBorder) / ((double)nStep); 
    for (double point = leftBorder + ((double)step) * 0.5; point < rightBorder; point += step)
        result += step / 6 * (function(point - step / 2) + 4 * function (point) + function (point + step / 2));
        
    return result;
}

double sqr (double parameter)
{
    return parameter * parameter;
}

static double func (double parameter)
{
    return log (cos (parameter) + parameter * parameter);
}

void OneThreadSimpson (void* resources)
{
    double leftBorder = ((IntInfo*)resources)->leftBorder;
    double rightBorder = ((IntInfo*)resources)->rightBorder;
    unsigned nStep = ((IntInfo*)resources)->nStep;
    double step = (rightBorder - leftBorder) / (nStep); 

    ((IntInfo*)resources)->result = SimpsonMethod (leftBorder, rightBorder, nStep, func);
}