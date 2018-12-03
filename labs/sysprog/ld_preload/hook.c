//#include <string.h>
#include <stdio.h>
//#include <math.h>
//#include <time.h>
#include <stdlib.h>

// Question 8 : write here the buggy function without errors

#define KNOT_TO_KMH 1.852

int knot_to_kmh_str(float not, size_t size, char * format, char * kmh_str)
{
    float kmh = KNOT_TO_KMH * not;
    snprintf(kmh_str, size, format, kmh);
    printf("Hooked you!\n");

    return kmh;
}



// Question 12 : write printf with a signal handler
