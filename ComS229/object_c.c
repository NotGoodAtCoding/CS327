#include "object_c.h"
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

int gen_obj(object_t *obj, int index, const char *name, char symbol, const char *type, short int color, const char *description, uint32_t speed, c_dice damage, int hit, int dodge, int def, int weight, int attr, int val, int num_allowed, int behavior){
  
  obj->index = index;
  obj->name = malloc(strlen(name) + 1);
  if(obj->name == NULL)
    return -1;
  memcpy(obj->name, name, strlen(name));
  obj->name[strlen(name)] = 0;
  
  obj->description = malloc(strlen(description) + 1);
  if(obj->description == NULL)
    return -1;
  memcpy(obj->description, description, strlen(description));
  obj->description[strlen(description)] = 0;
  
  obj->type = malloc(strlen(type) + 1);
  if(obj->type == NULL)
    return -1;
  memcpy(obj->type, type, strlen(type));
  obj->type[strlen(type)] = 0;
  
  obj->symb = symbol;
  obj->color_num = color;
  obj->speed = speed;
  
  obj->damage.base = damage.base;
  obj->damage.dice = damage.dice;
  obj->damage.sides = damage.sides;
  
  obj->hit = hit;
  obj->dodge = dodge;
  obj->def = def;
  obj->weight = weight;
  obj->attr = attr;
  obj->val = val;
  
  obj->num_allowed = num_allowed;
  obj->behavior = behavior;
  return 0;
}

void obj_print(object_t *obj, int x, int y){
  attron(COLOR_PAIR(obj->color_num));
  mvprintw(y, x, "%c", obj->symb);
  attroff(COLOR_PAIR(obj->color_num));
}

void destroy_obj(object_t *obj){
  free(obj->name);
  free(obj->description);
  free(obj->type);
}
