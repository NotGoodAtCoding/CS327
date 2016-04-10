#include "dungeon.h"
#include "prompt.h"
#include "object_c.h"

#include <ncurses.h>
#include <string.h>
#include <ctype.h>

WINDOW *prompt;
int len;
int height;

int equip_count[NUM_OBJECTS] = {0};

void create_prompt(dungeon *d);

void clear_prompt(dungeon *d);

void destroy_prompt(dungeon *d);

void list_equipment(dungeon *d);

void list_standby(dungeon *d);

/*
  Item behaviors:
  1. - can be equipped
  2. - can be collected
  3. - can be used to change carry_policies

  num_allowed - how many of this type can be 

*/

void pc_item_equip(dungeon *d){
  create_prompt(d);
  char *msg = "Please Select an item to equip.";
  mvwprintw(prompt, 3, (len - strlen(msg) )/2, "%s", msg); 
  list_standby(d);
  wrefresh(prompt);
  while(1){    
    int selection = getch();
    char blank[len-1];
    int j;
    for(j = 0; j < len-2; j++)
      blank[j] = ' ';
    blank[len-1] = 0;
    mvwprintw(prompt, height-2, 1, "%s", blank);

    if(selection == 27){
      destroy_prompt(d);
      return;
    }else if(selection >= '0' && selection <= '9'){
      int num = selection - '0';

      object_list_t *standby = &d->pc->attr.pc.standby;
      object_list_t *equiped = &d->pc->attr.pc.equiped;

      if(num < object_list_size(standby)){
	object_t *standby_temp = object_list_retrieve(standby, num);

	//If there is an available equipment slot and the standby slot has an item
	if( standby_temp->behavior == 1 && equip_count[standby_temp->index] <= standby_temp->num_allowed){

	  object_t to_equip;
	  object_list_remove(standby, num, &to_equip);

          if(equip_count[to_equip.index] < to_equip.num_allowed){
            equip_count[to_equip.index]++;
          }else{
            //iterate through equiped, swap things
            object_t to_unequip;
            int i;

            for(i = 0; i < object_list_size(equiped); i++){
              object_t *temp;
              temp = object_list_retrieve(equiped, i);
              if(temp->index == to_equip.index){
                object_list_remove(equiped, i, &to_unequip);
                d->pc->attr.pc.act_speed -= to_equip.speed;
                d->pc->speed = (d->pc->attr.pc.act_speed > 0) ? d->pc->attr.pc.act_speed : 1;
                temp = NULL;
                object_list_push(standby, &to_unequip);
                break;
              }
            }
          }

          // equip item
	  object_list_push(equiped, &to_equip);
          d->pc->attr.pc.act_speed += to_equip.speed;
          d->pc->speed = (d->pc->attr.pc.act_speed > 0) ? d->pc->attr.pc.act_speed : 1;
          if(to_equip.index == WEAPON_E)
            d->pc->attr.pc.weapon = 1;
	  clear_prompt(d);
	  mvwprintw(prompt, 3, (len - strlen(msg) )/2, "%s", msg); 
	  list_standby(d);
	  char can[80];
	  sprintf(can, "Item %s is being equipped.", standby_temp->name);
	  mvwprintw(prompt, height-2, (len - strlen(can) )/2, "%s", can);
	}else{
        char *can_not = "Item can not be equiped - try again.";
        mvwprintw(prompt, height-2, (len - strlen(can_not))/2, "%s", can_not);
	}
      }
      else{
        char *can_not = "Item can not be equiped - try again.";
        mvwprintw(prompt, height-2, (len - strlen(can_not))/2, "%s", can_not);
      }
      
    }
    wrefresh(prompt);
  }
}

void pc_item_drop(dungeon *d){
  create_prompt(d);
  char *msg = "Please Select an item to drop.";
  mvwprintw(prompt, 3, (len - strlen(msg) )/2, "%s", msg); 
  list_standby(d);
  wrefresh(prompt);
  while(1){    
    int selection = getch();
    char blank[len-1];
    int j;
    for(j = 0; j < len-2; j++)
      blank[j] = ' ';
    blank[len-1] = 0;
    mvwprintw(prompt, height-2, 1, "%s", blank);

    if(selection == 27){
      destroy_prompt(d);
      return;
    }else if(selection >= '0' && selection <= '9'){
      int num = selection - '0';

      object_list_t *standby = &d->pc->attr.pc.standby;

      if(num < object_list_size(standby)){
        object_t temp;
        object_list_remove(standby, num, &temp);
        place_object(d, d->pc->position.x, d->pc->position.y, &temp);

        clear_prompt(d);
        mvwprintw(prompt, 3, (len - strlen(msg) )/2, "%s", msg); 
        list_standby(d);
        char can[80];
        sprintf(can, "Item %s is being dropped.", temp.name);
	mvwprintw(prompt, height-2, (len - strlen(can) )/2, "%s", can);
        destroy_obj(&temp);
     
      }
      else{
        char *can_not = "Item can not be dropped - try again.";
        mvwprintw(prompt, height-2, (len - strlen(can_not))/2, "%s", can_not);
      }
      
    }
    wrefresh(prompt);
  }
}

void pc_item_unequip(dungeon *d){
  create_prompt(d);
  char *msg =  "Please Select an item to unequip.";
  mvwprintw(prompt, 3, (len - strlen(msg) )/2, "%s", msg); 
  list_equipment(d);
  wrefresh(prompt);
  while(1){    
    int selection = getch();
    char blank[len-1];
    int j;
    for(j = 0; j < len-2; j++)
      blank[j] = ' ';
    blank[len-1] = 0;
    mvwprintw(prompt, height-2, 1, "%s", blank);

    if(selection == 27){
      destroy_prompt(d);
      return;
    }else if(selection >= 'a' && selection <= 'l'){
      int num = selection - 'a';

      object_list_t *standby = &d->pc->attr.pc.standby;
      object_list_t *equiped = &d->pc->attr.pc.equiped;

 

      //If there is an available standby slot and the equipment slot has an item
      if(num < object_list_size(equiped) && object_list_size(standby) < d->pc->attr.pc.standby_max){
	object_t *equip_temp = object_list_retrieve(equiped, num);
        equip_count[equip_temp->index]--;
        object_t to_unequip;
        object_list_remove(equiped, num, &to_unequip);
        d->pc->attr.pc.act_speed -= to_unequip.speed;
        d->pc->speed = (d->pc->attr.pc.act_speed > 0) ? d->pc->attr.pc.act_speed : 1;
          if(to_unequip.index == WEAPON_E)
            d->pc->attr.pc.weapon = 0;
        object_list_push(standby, &to_unequip);
        clear_prompt(d);
        mvwprintw(prompt, 3, (len - strlen(msg) )/2, "%s", msg); 
        list_equipment(d);
        char can[80];
        sprintf(can, "Item %s is being unequipped.", equip_temp->name);
	mvwprintw(prompt, height-2, (len - strlen(can) )/2, "%s", can);
      }
      else{
        char *can_not = "Item can not be unequiped - try again.";
        mvwprintw(prompt, height-2, (len - strlen(can_not))/2, "%s", can_not);
      }
    }
    wrefresh(prompt);
  }
}

void pc_item_delete(dungeon *d){
  create_prompt(d);
  char *msg = "Please Select an item to delete.";
  mvwprintw(prompt, 3, (len - strlen(msg) )/2, "%s", msg); 
  list_standby(d);
  wrefresh(prompt);
  while(1){    
    int selection = getch();
    char blank[len-1];
    int j;
    for(j = 0; j < len-2; j++)
      blank[j] = ' ';
    blank[len-1] = 0;
    mvwprintw(prompt, height-2, 1, "%s", blank);

    if(selection == 27){
      destroy_prompt(d);
      return;
    }else if(selection >= '0' && selection <= '9'){
      int num = selection - '0';

      object_list_t *standby = &d->pc->attr.pc.standby;

      if(num < object_list_size(standby)){
        object_t temp;
        object_list_remove(standby, num, &temp);

        clear_prompt(d);
        mvwprintw(prompt, 3, (len - strlen(msg) )/2, "%s", msg); 
        list_standby(d);
        char can[80];
        sprintf(can, "Item %s is being deleted.", temp.name);
	mvwprintw(prompt, height-2, (len - strlen(can) )/2, "%s", can);
        destroy_obj(&temp);
     
      }
      else{
        char *can_not = "Item can not be deleted - try again.";
        mvwprintw(prompt, height-2, (len - strlen(can_not))/2, "%s", can_not);
      }
      
    }
    wrefresh(prompt);
  }
}

void create_prompt(dungeon* d){
  len = d->win_len * .8;
  height = d->win_height*.8;
  prompt = newwin(height, len, (d->win_height - height)/2, (d->win_len - len)/2);
  box(prompt, 0, 0);
  wrefresh(prompt);
}

void list_equipment(dungeon *d){
  int equip_x = 2;
  int equip_y = 5;
  int i;

  object_list_t *equiped = &d->pc->attr.pc.equiped;
  int num_equiped = object_list_size(equiped);
  for(i = 0; i < 12; i++){
    if(i < num_equiped){
      object_t *temp = object_list_retrieve(equiped, i);
      mvwprintw(prompt, equip_y+i, equip_x, "%c. %s", 'a'+i, temp->name);
    }
    else
      mvwprintw(prompt, equip_y+i, equip_x, "%c. <empty>", 'a'+i);
  }
  wrefresh(prompt);
  
}

void list_standby(dungeon *d){
  int standby_x = 2;
  int standby_y = 5;
  int i;

  object_list_t *standby = &d->pc->attr.pc.standby;
  int num_standby = object_list_size(standby);

  for(i = 0; i < d->pc->attr.pc.standby_max; i++){
    if(i < num_standby){
      object_t *temp = object_list_retrieve(standby, i);
      mvwprintw(prompt, standby_y+i, standby_x, "%d. %s", i, temp->name);
    }
    else
      mvwprintw(prompt, standby_y+i, standby_x, "%d. <empty>", i);
  }
  wrefresh(prompt);
  
}
void clear_prompt(dungeon *d){
  delwin(prompt);
  create_prompt(d);
}

void destroy_prompt(dungeon *d){
  delwin(prompt);
  print_dungeon(d);
}
