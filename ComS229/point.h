#ifndef __point
#define __point

#include <math.h>

typedef struct point_type{
  int x;
  int y;
}point_type;

double distance_between(point_type a, point_type b);

#endif