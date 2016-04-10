#include "c_dice.h"
#include <stdlib.h>

int dice_roll(c_dice to_roll){
  if(to_roll.sides == 0 || to_roll.dice == 0)
    return to_roll.base;
  int total = 0, i;
  for(i = 0; i < to_roll.dice; i++){
    total += (rand() % (to_roll.sides)) + 1;
  }
  return total + to_roll.base;
}
