#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "heap.h"

//Converts a distance to an appropriate ascii code.
//No check for out of bounds.
int distance_to_ascii(int dist){
  return (dist > 35) ? 29+dist : ((dist >9) ? 87+dist : 48+dist);
}

int main(){
  int i=0;
  for ( ; i<62; i++){
    putchar(distance_to_ascii(i));
  }
  putchar('\n');
  return 0;
}
