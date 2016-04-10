#ifndef DICE_PARAM_H
#define DICE_PARAM_H

#include <iostream>

using namespace std;

class dice_param{
    

    int base;
    int dice;
    int sides;
    
public:
    dice_param(void){
        base = 0;
        dice = 0;
        sides = 0;
    }

    dice_param(int base, int dice, int sides){
        this->base = base;
        this->dice = dice;
        this->sides = sides;
    }
    
    friend ostream &operator<<(ostream &o, const dice_param &input);
    dice_param &operator=(const dice_param &input);
    
    void set_base(int base);
    
    void set_dice(int dice);
    
    void set_sides(int sides);
    
    int get_base(void);

    int get_dice(void);

    int get_sides(void);
    
    int roll(void);
};
#endif
