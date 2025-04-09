#ifndef SPHERE
#define SPHERE

#include <math.h>

#include "../tuple/tuple.h"
#include "../ray/ray.h"

typedef struct 
{
    Tuple center;
    double radius;
} Sphere;

typedef struct
{
    unsigned int count;
    double t_vals[2];
    Sphere object;
} Isect;


Sphere sphere(Tuple, double);
Isect intersect(Sphere, Ray);

#endif 