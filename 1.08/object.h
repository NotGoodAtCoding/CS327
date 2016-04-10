//
// Created by Stefan Kraus on 3/29/2016.
//

#ifndef CS327_OBJECT_H
#define CS327_OBJECT_H

#include <string>

#include "dice.h"
#include "descriptions.h"
#include "dims.h"

#define WEAPON_SYMB '|'
#define OFFHAND_SYMB ')'
#define RANGED_SYMB '}'
#define ARMOR_SYMB '['
#define HELMET_SYMB ']'
#define CLOAK_SYMB '('
#define GLOVES_SYMB '{'
#define BOOTS_SYMB '\\'
#define RING_SYMB '='
#define AMULET_SYMB '"'
#define LIGHT_SYMB '_'
#define SCROLL_SYMB '~'
#define BOOK_SYMB '?'
#define FLASK_SYMB '!'
#define GOLD_SYMB '$'
#define AMMUNITION_SYMB '/'
#define FOOD_SYMB ','
#define WAND_SYMB '-'
#define CONTAINER_SYMB '%'
#define STACK_SYMB '&'

using namespace std;

typedef struct dungeon dungeon_t;

class object {
public:
    string name,description;
    pair_t position;
    object_type_t type;
    char symbol;
    uint32_t color;
    int32_t hit_bonus,dge_bonus,def_bonus,weight,spd_bonus,value,spec_attr;
    dice dam_bonus;

    object(void);

};
void gen_objects(dungeon_t *d);
int8_t *object_get_pos(const object *o);
uint32_t object_get_color(const object *o);
char object_get_symbol(const object *o);

#endif //CS327_OBJECT_H
