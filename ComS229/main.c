#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "dungeon.h"
#include "player.h"
#include "p_queue.h"
#include "monster_c.h"
#include "player.h"

#include "parser.h"

int main(int argc, char *argv[]) {

  // Setup for ncurses
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();

  srand(time(NULL));

  // Important varialbes.
  dungeon d;
  player_t pc;
  p_queue keeper;
  int Xdim = 160;
  int Ydim = 96;
  int num_mon = 20;
  int num_obj = 15;
  int num_rooms = 12;

  // Generate PC
  gen_pc(&pc);

  // variables for templates
  void *object_templates;
  void *monster_templates;

  // Variables for file paths
  char *home = getenv("HOME");
  char *dir = "/.rlg229";
  char *monster_file = "/monster_desc.txt";
  char *object_file = "/object_desc.txt";
  char *monster_path;
  char *object_path;

  // Setup for file paths
  monster_path =
      malloc(strlen(home) + strlen(dir) + strlen(monster_file) + 1);
  strcpy(monster_path, home);
  strcat(monster_path, dir);

  object_path =
      malloc(strlen(home) + strlen(dir) + strlen(object_file) + 1);

  strcpy(object_path, monster_path);

  // Finalize file paths
  strcat(monster_path, monster_file);
  strcat(object_path, object_file);

  // Parse templates
  parse_monster_templates(&monster_templates, monster_path);
  parse_object_templates(&object_templates, object_path);

  free(monster_path);
  free(object_path);

  int return_value = 0;


  while(return_value != 1 && return_value != 3){

    // Generate Dungeon
    //if(load_dungeon(&d, &pc) == -1)
	int ret;
	do{
   		ret = generate_dungeon(&d, &pc, num_rooms, num_mon, monster_templates, num_obj, object_templates, Xdim, Ydim);
	}while(ret == -2);
	if (ret == -1){
		destroy_player(&pc);
  		destroy_monster_templates(monster_templates);
  		destroy_object_templates(object_templates);
  		endwin();
		printf("Wait a second, then try again - bad random number seed\n");
		return 0;
	}
    
    // Initialize player Queue
    init_q(&keeper, sizeof(player_t *));

    // Push all players onto Queue with same turn
    player_t *temp_pc = &pc;
    push(&keeper, &temp_pc, 1);
    int i;
    for(i = 0; i < d.num_monsters; i++){
      player_t *temp = &(d.monsters[i]);
      push(&keeper, &temp, 1);
    }

    // Main game loop
    while(1){
      player_t *temp;
      // Pop newest player from stack
      pop(&keeper, &temp);
    
      // If player is dead, do not add back to Queue - gone
      if(has_att(temp, DEAD))
	continue;
      
      // refresh screen if player is PC
      if(temp->index == 0)
	print_dungeon(&d);

      // update player's position
      return_value = move_ocupant(&d, temp);
      
      // Check return value for outcome of move
      if(return_value == 1 || return_value == 2 || return_value == 3)
	break;
      
      // Update player's next turn, add to Queue
      temp->turn += 100/(temp->speed);
      push(&keeper, &temp, temp->turn);
    }
    reset_pc(&pc);
    destroy_dungeon(&d);
    destroy(&keeper);
  }


  int j;
  for(j = 0; j < d.num_monsters; j++){
    //init_pair(10, COLOR_WHITE, COLOR_BLACK);
    //attron(COLOR_PAIR(10));
    //  mvprintw(j+5, 0, "Parsed: %s",(d.monsters[j]).attr.npc.name);
    //    destroy_player(&(d.monsters[j]));
  }

  
  // Cleanup
  destroy_player(&pc);
  destroy_monster_templates(monster_templates);
  destroy_object_templates(object_templates);
  endwin();

  /*
  int Xdim = 160;
  int Ydim = 96;
  int room_num = 12;
  int monster_num = 10;
  int will_save = 1;
  int will_load = 1;
  int ret = 0, gen = 0, i;

  int debug = 0;

  dungeon my_dungeon;
  player_t pc;
  p_queue keeper;

  gen_pc(&pc);
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  srand(time(NULL));

  for(i = 1; i < argc; i++){
    if(!strcmp(argv[i], "--save"))
       will_save = 1;
    else if(!strcmp(argv[i], "--load"))
      will_load = 1;
    else if(!strcmp(argv[i], "--nummon"))
      sscanf(argv[i+1], "%d", &monster_num);
  }


  while(!(ret == 1 || ret == 3)){

    if(will_load && ret == 0){
      if(load_dungeon(&my_dungeon, &pc) == -1){
        fprintf(stderr,"Problem reading load file.\n");
        gen = generate_dungeon(&my_dungeon, &pc, room_num, monster_num, Xdim,
  Ydim);
        //return -1;
      }
      fprintf(stderr, "returned from load_duneon\n\r");
    }

    else{
      do{
        gen = generate_dungeon(&my_dungeon, &pc, room_num, monster_num, Xdim,
  Ydim);
        if(gen == -2)
          destroy_dungeon(&my_dungeon);
      }while(gen == -2);

      if(gen == -1){
        printf("Problem with malloc\n");
        destroy_dungeon(&my_dungeon);
        return -1;
      }
    }

    init_q(&keeper, sizeof(player_t *));

    player_t *temp_pc = &pc;
    push(&keeper, &temp_pc, 1);

    for(i = 0; i < my_dungeon.num_monsters; i++){
      player_t *temp = &(my_dungeon.monsters[i]);
      push(&keeper, &temp, 1);
    }


    while(1){
      player_t *temp;
      pop(&keeper, &temp);
      if(has_att(temp, DEAD))
        continue;
      if(temp->index == 0 )

        print_dungeon(&my_dungeon);
          if(debug == 1)
      ret = move_ocupant(&my_dungeon, temp);
            debug = 1;
      if(ret == 1 || ret == 2 || ret == 3)
        break;

      temp->turn += 100/(temp->speed);
      push(&keeper, &temp, temp->turn);
      // usleep(7000);
    }


    if(will_save && ret == 3)
      save_dungeon(&my_dungeon);

    if(ret == 1){
      char *home = getenv("HOME");
      char *dir = malloc(strlen(home) + strlen("/.rlg229") + strlen("/dungeon")
  + 1);
      // char *dir;
      //  asprintf(&dir, "%s/.rlg229/dungeon", home);

      strcpy(dir, home);
      strcat(dir, "/.rlg229");
      strcat(dir, "/dungeon");
      remove(dir);
      FILE *fd = fopen("test", "w");
      fprintf(fd, "removing %s\n", dir);
      fclose(fd);
      free(dir);
    }


    destroy_dungeon(&my_dungeon);

    reset_pc(&pc);
    destroy(&keeper);

  }
  endwin();
  printf("Simulation stopped\n");




  //  endwin();
  */
  return 0;
}
