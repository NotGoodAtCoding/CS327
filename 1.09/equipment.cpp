//
// Created by Stefan Kraus on 4/10/2016.
//

#include "equipment.h"
#include "dungeon.h"
#include "object.h"
#include <ncurses.h>
#include <curses.h>
#include <cstring>

#define ESC            27
#define MENU_HEIGHT    18
#define MENU_WIDTH     40
#define MENU_X         20
#define MENU_Y         2

WINDOW *prompt;

void init_menu(){
    prompt = newwin(MENU_HEIGHT, MENU_WIDTH, MENU_Y, MENU_X);
    box(prompt, 0, 0);
    wrefresh(prompt);
}

void init_inventory(dungeon *d){
    memset(d->pc->carry, 0, sizeof(d->pc->carry));
    memset(d->pc->equipment, 0, sizeof(d->pc->equipment));
}

void destroy_menu(){
    delwin(prompt);
}

void pc_equip_menu(dungeon *d){
    init_menu();
    char msg[40] = "Select slot 0-9 to equip or (q)uit";
    mvwprintw(prompt, 2, (DUNGEON_X/2 - strlen(msg))/2, "%s", msg);
    list_pc_carry(d);
    wrefresh(prompt);

    while(1){
        int in = getch();
        if(in == ESC || in == 'q'){
            destroy_menu();
            break;
        } else if(in >= '0' && in <= '9'){
            int slot = in - '0';

            if(!d->pc->carry[slot]) continue;

            object *to_equip = d->pc->carry[slot];

            if(!d->pc->equipment[to_equip->get_type()-1] || //Slot is empty
                (to_equip->get_type()-1 == 10 &&            //OR item is ring and ring2 is empty
                 !d->pc->equipment[to_equip->get_type()])){

                d->pc->equipment[to_equip->get_type()-1] = to_equip;
                d->pc->carry[slot] = 0;
            }

            break;
        }
    }
}

void pc_unequip_menu(dungeon *d){
    init_menu();
    char msg[40] = "Select slot 0-9 to unequip or (q)uit";
    mvwprintw(prompt, 2, (DUNGEON_X/2 - strlen(msg))/2, "%s", msg);
    list_pc_equipment(d);
    wrefresh(prompt);

    while(1){
        int in = getch();
        if(in == ESC || in == 'q'){
            destroy_menu();
            break;
        } else if(in >= 'a' && in <= 'l'){
            int slot = in - 'a'; //offset to 0-11

            if(!d->pc->equipment[slot]) continue; //out of bounds? try again

            object *to_unequip = d->pc->equipment[slot];
            d->pc->equipment[slot] = 0;
            //Attempt to place in carry
            int i=0;
            while(i < CARRY_MAX){
                if(!d->pc->carry[i]){
                    d->pc->carry[i] = to_unequip;
                    break;
                }
                i++;
            }
            //Carry is full, drop on ground
            if(i == CARRY_MAX){
                d->objmap[d->pc->position[dim_y]][d->pc->position[dim_x]] = to_unequip;
            }

            break;
        }
    }
}

void pc_drop_menu(dungeon *d){
    init_menu();
    char msg[40] = "Select slot 0-9 to drop or (q)uit";
    mvwprintw(prompt, 2, (DUNGEON_X/2 - strlen(msg))/2, "%s", msg);
    list_pc_carry(d);
    wrefresh(prompt);

    while(1){
        int in = getch();
        if(in == ESC || in == 'q'){
            destroy_menu();
            break;
        } else if(in >= '0' && in <= '9'){
            int slot = in - '0';

            if(!d->pc->carry[slot]) continue;

            object *to_drop = d->pc->carry[slot];

            d->objmap[d->pc->position[dim_y]][d->pc->position[dim_x]] = to_drop;
            d->pc->carry[slot] = 0;
            break;
        }
    }
}

void pc_expunge_menu(dungeon *d){
    init_menu();
    char msg[40] = "Select slot 0-9 to OBLITERATE, (q)uit";
    mvwprintw(prompt, 2, (DUNGEON_X/2 - strlen(msg))/2, "%s", msg);
    list_pc_carry(d);
    wrefresh(prompt);

    while(1){
        int in = getch();
        if(in == ESC || in == 'q'){
            destroy_menu();
            break;
        } else if(in >= '0' && in <= '9'){
            int slot = in - '0';

            if(!d->pc->carry[slot]) continue;

            d->pc->carry[slot] = 0;
            break;
        }
    }
}

void pc_inventory_menu(dungeon *d){
    init_menu();
    char msg[40] = "INVENTORY ... (q)uit";
    mvwprintw(prompt, 2, (DUNGEON_X/2 - strlen(msg))/2, "%s", msg);
    list_pc_carry(d);
    wrefresh(prompt);

    while(1){
        int in = getch();
        if(in == ESC || in == 'q'){
            destroy_menu();
            break;
        }
    }
}

void pc_equipment_menu(dungeon *d){
    init_menu();
    char msg[40] = "EQUIPMENT ... (q)uit";
    mvwprintw(prompt, 2, (DUNGEON_X/2 - strlen(msg))/2, "%s", msg);
    list_pc_equipment(d);
    wrefresh(prompt);

    while(1){
        int in = getch();
        if(in == ESC || in == 'q'){
            destroy_menu();
            break;
        }
    }
}

void pc_inspection_menu(dungeon *d){
    init_menu();
    char msg[40] = "Select slot 0-9 to INSPECT or (q)uit";
    mvwprintw(prompt, 1, (DUNGEON_X/2 - strlen(msg))/2, "%s", msg);
    list_pc_carry(d);
    wrefresh(prompt);

    while(1){
        int in = getch();
        if(in == ESC || in == 'q'){
            destroy_menu();
            break;
        } else if(in >= '0' && in <= '9'){
            int slot = in - '0';

            if(!d->pc->carry[slot]) continue;

            mvwprintw(prompt, 2, 1, "%s", d->pc->carry[slot]->get_desc());
            wrefresh(prompt);
        }
    }
}

void list_pc_equipment(dungeon *d){
    int x_offset = 2;
    int y_offset = 4;
    int i=0;

    for( ; i<12; i++){
      if(d->pc->equipment[i]){
        mvwprintw(prompt, i + y_offset, x_offset, "%c %s", i+'a', d->pc->equipment[i]->get_name());
      } else {
        mvwprintw(prompt, i + y_offset, x_offset, "%c Empty", i+'a');
      }
    }
    wrefresh(prompt);
}

void list_pc_carry(dungeon *d){
    int x_offset = 2;
    int y_offset = 4;
    int i=0;

    for( ; i<10; i++){
        if(d->pc->carry[i]){
            mvwprintw(prompt, i + y_offset, x_offset, "%d %s", i, d->pc->carry[i]->get_name());
        } else {
            mvwprintw(prompt, i + y_offset, x_offset, "%d Empty", i);
        }

    }
    wrefresh(prompt);
}

void attempt_pickup(dungeon *d){
    object *to_equip = d->objmap
    [d->pc->position[dim_y]]
    [d->pc->position[dim_x]];

    int i=0;
    while(i < CARRY_MAX){
        if(!d->pc->carry[i]){
            d->pc->carry[i] = to_equip;
            d->objmap[d->pc->position[dim_y]][d->pc->position[dim_x]] = 0;
            break;
        }
        i++;
    }
}