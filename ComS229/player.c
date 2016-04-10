#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "player.h"

static int num = 1;

void gen_pc(player_t *pc){
  pc->attr.pc.weapon = 0;
  pc->index = 0;
  pc->symbol = '@';
  pc->attr.pc.act_speed = 10;
  pc->speed = pc->attr.pc.act_speed;
  pc->turn = 0;
  reset_att(pc, DEAD);
  pc->color_num = COLOR_WHITE;
  pc->attr.pc.standby_max = 10;
  object_list_init(&(pc->attr.pc.standby), sizeof(object_t));
  object_list_init(&(pc->attr.pc.equiped), sizeof(object_t));
  pc->hitpoints = 100;
  pc->damage.base = 0;
  pc->damage.dice = 1;
  pc->damage.sides = 4;
}

void reset_pc(player_t *pc){
  pc->turn = 0;
}

void set_position(player_t *p, point_type position){
  p->position = position;
}

int set_index(player_t *p, int index){
  if(index >= num){
    return 1;
  }
  p->index = index;
  return 0;
}

void set_next_index(int index){
  num = index;
}

int get_next_index(void){
  return num;
}

void set_turn(player_t *p, int turn){
  p->turn = turn;
}

int gen_npc(player_t *npc, const char *name, char symbol, short int color, const char *description, int speed, c_dice damage, int health){
  npc->index = num++;
  npc->speed = speed;
  npc->hitpoints = health;
  npc->damage = damage;
  npc->turn = 0;
  npc->attr.npc.last_seen.x = 0xffff;
  npc->attr.npc.last_seen.y = 0xffff;
  npc->attr.npc.attr = 0;
  
  npc->attr.npc.name = malloc( strlen(name)+1);
  npc->attr.npc.description = malloc( strlen(description)+1);
  
  if(npc->attr.npc.name == NULL)
    return -1;
  
  memcpy(npc->attr.npc.name, name, strlen(name)); 
  npc->attr.npc.name[strlen(name)] = 0;

  if(npc->attr.npc.description == NULL)
    return -1;
  
  memcpy(npc->attr.npc.description, description, strlen(description));
  npc->attr.npc.description[strlen(description)]= 0;
  
  npc->symbol = symbol;
  npc->color_num = color;
  return 0;
}

void destroy_player(player_t *p){
  if(p->index != 0){
    free(p->attr.npc.name);
    free(p->attr.npc.description);
  }
  else{
    object_list_destroy(&(p->attr.pc.standby));
    object_list_destroy(&(p->attr.pc.equiped));
  }
}

void player_print(player_t *player, int x, int y){
  attron(COLOR_PAIR(player->color_num));
  mvprintw(y, x, "%c", player->symbol);
  attroff(COLOR_PAIR(player->color_num));
}

int player_pickup(player_t *pc, object_t *obj){
  if(object_list_size(&(pc->attr.pc.standby)) < pc->attr.pc.standby_max){
    object_list_push(&(pc->attr.pc.standby), obj);
    return 0;
  }
  else
    return -1;
}

int player_attack(player_t *pc, player_t *monster){
  
  // Update PC hitpoints
  pc->hitpoints -= dice_roll(monster->damage);

  // Monster wins
  if(pc->hitpoints < 0)
    return 2;

  //calculate PC damage
  int pc_damage_cal = pc->attr.pc.weapon ? 0 : dice_roll(pc->damage);
  int i;
  for(i = 0; i < object_list_size(&(pc->attr.pc.equiped)); i++){
    object_t *temp = object_list_retrieve(&(pc->attr.pc.equiped), i);
    pc_damage_cal += dice_roll(temp->damage);
  }
  
  // Update monster hitpoints
  monster->hitpoints -= pc_damage_cal;
  
  // PC wins
  if(monster->hitpoints < 0 )
    return 1;

  // Stalemate
  return 0;
}
