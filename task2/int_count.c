#include "int_count.h"

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