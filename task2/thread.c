#include <stdio.h>
#include <stdlib.h>
#include "int_count.h"

/*calculating ln(cos(x)+x^2) from 10 to 100)*/

int main()
{
    printf ("%lg\n", SimpsonMethod (2,4,100,sqr));
    return 0;
}