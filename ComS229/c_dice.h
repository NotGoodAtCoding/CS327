#ifndef _C_DICE
#define _C_DICE


typedef struct dice{
  int base;
  int dice;
  int sides;
}c_dice;

int dice_roll(c_dice to_roll);

#endif
