//
// Created by Stefan Kraus on 3/29/2016.
//

#include "monster.h"

#include <ncurses.h>
#include <stdlib.h>

monster::monster(void) {}

monster::monster(char *name, char *desc, char symb){
    this->name = name;
    this->desc = desc;
    this->symb = symb;
}

void monster::set_hp(dice in){
    this->hp = in;
}

void monster::set_speed(dice in){
    this->speed = in;
}

void monster::set_dam(dice in){
    this->dam = in;
}

void monster::set_desc(std::string desc){
    this->desc = desc;
}

void monster::add_ability(string ability){
    abilities.push_back(ability);
}

void monster::set_abilities(vector<string> ability){
    this->abilities = abilities;
}

void monster::set_color(string color) {
    if (color == "RED" || color == "GREEN" || color == "BLUE"
        || color == "CYAN" || color == "YELLOW" || color == "MAGENTA"
        || color == "WHITE" || color == "BLACK"){
        this->color = color;
    }else{
        this->color = "WHITE";
    }
}