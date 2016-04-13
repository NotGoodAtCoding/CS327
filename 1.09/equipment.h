//
// Created by Stefan Kraus on 4/10/2016.
//

#ifndef CS327_EQUIPMENT_H
#define CS327_EQUIPMENT_H
#include "dungeon.h"

void init_menu();
void init_inventory(dungeon *d);
void pc_equip_menu(dungeon *d);
void pc_unequip_menu(dungeon *d);
void pc_drop_menu(dungeon *d);
void pc_expunge_menu(dungeon *d);
void pc_inventory_menu(dungeon *d);
void pc_equipment_menu(dungeon *d);
void pc_inspection_menu(dungeon *d);
void destroy_menu();
void list_pc_equipment(dungeon *d);
void list_pc_carry(dungeon *d);
void attempt_pickup(dungeon *d);

#endif //CS327_EQUIPMENT_H
