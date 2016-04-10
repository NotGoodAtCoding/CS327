#ifndef __room
#define __room

#include "point.h"

/*
typedef struct point_type{
  int x;
  int y;
}
point_type;
*/
typedef struct room{
  int room_id;
  point_type corner[4];
  int Xlen;
  int Ylen;
  int area;
}room;


//double distance_between(point_type a, point_type b);

double hausdorf_between(room a, room b);

int generate_room(room *r, int minX, int minY, int area);

void set_coord(room *r, int x, int y);

void find_center(room *r, point_type *p);

#endif
