//
// Created by Stefan Kraus on 3/29/2016.
//

#ifndef CS327_OBJECT_H
#define CS327_OBJECT_H

#include <string>

#include "dice.h"

using namespace std;
class object {
public:
    string name;
    string description;
    string type;
    string color;
    dice hit_bonus;
    dice dam_bonus;
    dice dge_bonus;
    dice def_bonus;
    dice weight;
    dice spd_bonus;
    dice value;
    dice spec_attr;

    object(void);

    void set_hit_bonus(dice in);
    void set_dam_bonus(dice in);
    void set_dge_bonus(dice in);
    void set_def_bonus(dice in);
    void set_weight(dice in);
    void set_spd_bonus(dice in);
    void set_spc_value(dice in);
    void set_color(string color);
    void set_type(string type);
    void set_desc(string desc);

};


#endif //CS327_OBJECT_H
