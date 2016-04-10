#include "point.h"

double distance_between(point_type a, point_type b){
  double Ydiff = b.y - a.y;
  double Xdiff = b.x - a.x;
  return sqrt(pow(Ydiff, 2) + pow(Xdiff, 2));
}
