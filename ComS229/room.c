#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "room.h"

/*
double distance_between(point_type a, point_type b){
  double Ydiff = b.y - a.y;
  double Xdiff = b.x - a.x;
  return sqrt(pow(Ydiff, 2) + pow(Xdiff, 2));
}
*/
double hausdorf_between(room a, room b){
  int i, j;
  int small_distance;
  int large_distance = 0;
  
  for( j = 0; j < 4; j++){
    small_distance = INT_MAX;
    //find smallest distance between point a and points of b
    for(i = 0; i < 4; i++){
      int temp = distance_between(a.corner[j], b.corner[i]);
      if(small_distance > temp)
        small_distance = temp;
    }
    if(large_distance < small_distance)
      large_distance = small_distance;
  }
  return large_distance;
}

void set_coord(room *r, int x, int y){
  r->corner[0].x = x;
  r->corner[0].y = y;
  r->corner[1].x = x+r->Xlen;
  r->corner[1].y = y;
  r->corner[2].x = x+r->Xlen;
  r->corner[2].y = y+r->Ylen;
  r->corner[3].x = x;
  r->corner[3].y = y+r->Ylen;
}

void find_center(room *r, point_type *p){
  p->x = r->corner[0].x + r->Xlen/2;
  p->y = r->corner[0].y + r->Ylen/2;
}

int generate_room(room *r, int minX, int minY, int area){
  static int id = 0;
  int short_side, long_side, temp;
  int new_short_side, try = 0;
  int allowed_tries = 2000;
  //printf("Trying to create room with area of %d\n", area);
  if(minY-minX > 0){
	  long_side = minY;
	  short_side = minX;
  }else{
	  short_side = minY;
	  long_side = minX;
  }
  r->room_id = id++;
  do{
    temp = (area/short_side)*.5;
	  new_short_side = rand()%temp;
    //printf("on try %d, calculate new_short_side to be %d\n", try, new_short_side);
	  try++;
  }while(new_short_side < long_side && try < allowed_tries);
  if(try < allowed_tries){
  	r->Xlen = new_short_side;
  	r->Ylen = area/new_short_side;
  	r->area = r->Xlen * r->Ylen;
    set_coord(r, 0, 0);
    //printf("Success!: Xlen = %d, Ylen = %d\n", r->Xlen, r->Ylen);
    return 0;
  }
  else{
    //printf("Failed on generation of room %d\n", id-1);
    return 1;
  }
  //printf("generating room %d at address %p\n", id++, r);
}
