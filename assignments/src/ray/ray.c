#include "ray.h"
#include "../tuple/tuple.h"

Ray
ray (Tuple origin, Tuple direction)
{
  Ray new_ray;
  new_ray.origin = origin;
  new_ray.direction = direction;
  return new_ray;
}

Tuple
position (Ray r, double t)
{
  Tuple distance = scalarMultTuple (t, r.direction);
  return addTuples (r.origin, distance);
}
