#ifndef _OBJECT_C_H
#define _OBJECT_C_H

#include "c_dice.h"

#include <stdint.h>

typedef struct object_s{
  int index;
  char *name;
  char *description;
  char symb;
  char *type;
  short int color_num;
  int hit;
  c_dice damage;
  int dodge;
  int def;
  int weight;
  uint32_t speed;
  int attr;
  int val;
  int behavior;
  int num_allowed;
}object_t;

typedef enum object_e{
  WEAPON_E,
  OFFHAND_E,
  RANGED_E,
  ARMOR_E,
  HELMET_E,
  CLOAK_E,
  GLOVES_E,
  BOOTS_E,
  AMULET_E,
  LIGHT_E,
  RING_E,
  SCROLL_E,
  BOOK_E,
  FLASK_E,
  GOLD_E,
  AMMUNITION_E,
  FOOD_E,
  WAND_E,
  CONTAINER_E,
  NUM_OBJECTS
}object_e;

#ifdef __cplusplus
extern "C" {
#endif

int generate_objects(object_t **objects, void * descriptions, int num_objects);

int gen_obj(object_t *obj, int index, const char *name, char symbol, const char *type, short int color, const char *description, uint32_t speed, c_dice damage, int hit, int dodge, int def, int weight, int attr, int val, int num_allowed, int behavior);

void obj_print(object_t *obj, int x, int y);

void destroy_obj(object_t *obj);

#ifdef __cplusplus
}
#endif

#endif
