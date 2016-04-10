//
// Created by Stefan Kraus on 3/29/2016.
//

#include "object.h"

object::object(void){}

void object::set_hit_bonus(dice in){
    this->hit_bonus = in;
}
void object::set_dam_bonus(dice in){
    this->dam_bonus = in;
}
void object::set_dge_bonus(dice in){
    this->dge_bonus = in;
}
void object::set_def_bonus(dice in){
    this->def_bonus = in;
}
void object::set_weight(dice in){
    this->weight = in;
}
void object::set_spd_bonus(dice in){
    this->spd_bonus = in;
}
void object::set_spc_value(dice in){
    this->spec_attr = in;
}
void object::set_color(string color){
    if (color == "RED" || color == "GREEN" || color == "BLUE"
        || color == "CYAN" || color == "YELLOW" || color == "MAGENTA"
        || color == "WHITE" || color == "BLACK"){
        this->color = color;
    }else{
        this->color = "WHITE";
    }
}
void object::set_type(string type){
    this->type = type;
}
void object::set_desc(string desc){
    this->description = desc;
}