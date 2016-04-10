#include "dice_param.h"
#include <stdlib.h>


ostream& operator<<(ostream &o, const dice_param &input){
    return o << input.base << "+" << input.dice << "d" << input.sides;
}

dice_param& dice_param::operator=(const dice_param &input){
    this->base = input.base;
    this->dice = input.dice;
    this->sides = input.sides;
    
    return *this;
}

void dice_param::set_base(int base){
    this->base = base;
}

void dice_param::set_dice(int dice){
    this->dice = dice;
}

void dice_param::set_sides(int sides){
    this->sides = sides;
}

int dice_param::get_base(void){
    return base;
}

int dice_param::get_dice(void){
    return dice;
}

int dice_param::get_sides(void){
    return sides;
}

int dice_param::roll(void){
  if(this->sides == 0 || this->dice == 0)
    return this->base;
  int total = 0, i;
  for(i = 0; i < this->dice; i++){
    total += (rand() % (this->sides)) + 1;
  }
  return total + this->base;
}
