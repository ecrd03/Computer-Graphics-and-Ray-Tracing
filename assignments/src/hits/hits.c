#include "hits.h"
#include <stdarg.h>
#include <stdlib.h>

IRec
intersection (double t, Sphere s)
{
  IRec iRec;
  iRec.t = t;
  iRec.object = s;
  return iRec;
}

IRecs
intersections (unsigned int count, ...)
{
  va_list args;
  va_start (args, count);
  IRecs xs;
  xs.count = count;
  xs.records = (IRec *) malloc (sizeof (IRec) * count);

  for (unsigned int i = 0; i < count; i++)
    {
      xs.records[i] = va_arg (args, IRec);
    }

  va_end (args);
  return xs;
}

IRec
hit (IRecs irecs)
{
  IRec closest;
  closest.t = INVALID_T;


  for (unsigned int i = 0; i < irecs.count; i++)
    {
      if (irecs.records[i].t > 0
	  && (closest.t == INVALID_T || irecs.records[i].t < closest.t))
	{
	  closest = irecs.records[i];
	}
    }


  return closest;
}
