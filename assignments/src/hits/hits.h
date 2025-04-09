#ifndef HITS
#define HITS

#include <stdarg.h>
#include "../surfaces/sphere.h"

#define INVALID_T -999

typedef struct 
{
    double t;
    Sphere object;
} IRec;

typedef struct
{
    unsigned int count;
    IRec* records;
} IRecs;


IRec intersection(double, Sphere);
IRecs intersections(unsigned int, ...);
IRec hit(IRecs);

#endif