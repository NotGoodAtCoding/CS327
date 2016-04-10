#include "dice.h"
#include <stdlib.h>


dice::dice(){
    dice(0,0,0);
};

dice::dice(int base, int num, int sides){
    this->base = base;
    this->num = num;
    this->sides = sides;
}

int roll(dice d){
    if(d.sides == 0 || d.num == 0)
        return d.base;
    int total = 0, i;
    for(i = 0; i < d.num; i++){
        total += (rand() % (d.sides)) + 1;
    }
    return total + d.base;
}
