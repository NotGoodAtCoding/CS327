#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

#include "dungeon.h"
#include "path.h"

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "       [-i|--image <pgm>] [-s|--save]\n",
          name);

  exit(-1);
}

//ADDED
/* Visibility, pokemon style: only straight lines in x or y */
int visible(dungeon_t *d, monster_t mon, pc_t pc){
  uint8_t i;
  if(mon.position[dim_x] == pc.position[dim_x]){
    for(i=min(mon.position[dim_y], pc.position[dim_y]);
        i<max(mon.position[dim_y], pc.position[dim_y]); i++){
      if(d->map[i][mon.position[dim_x]] == ter_wall ||
         d->map[i][mon.position[dim_x]] == ter_wall_immutable){
        return 0; //No line of sight
      }
    }
  }
  if(mon.position[dim_y] == pc.position[dim_y]){
    for(i=min(mon.position[dim_x], pc.position[dim_x]);
        i<max(mon.position[dim_x], pc.position[dim_x]); i++){
      if(d->map[i][mon.position[dim_y]] == ter_wall ||
         d->map[i][mon.position[dim_y]] == ter_wall_immutable){
        return 0; //No line of sight
      }
    }
  }
  return 1;
}


//ADDED
int move_monster(dungeon_t *d, monster_t mon, pc_t pc){
  //Decide to move
  if(!( visible(d, mon, pc) ||         //Line of Sight
        mon.position != mon.target ||  //Intelligence
        mon.attributes & 2 ||          //Telepathy
        mon.attributes & 8 )){         //Eratic
    return 1; //Did not move
  }

  //decide where to move, order dependent
  if(mon.attributes & 8 && rand() & 1){//Erratic, 50% chance

  }

  if(mon.attributes & 1){//Intelligence

  }
  if(mon.attributes & 2){ //Telepathy
    *mon.target = *pc.position;
  }
  if(mon.attributes & 4){//Tunneling
    
  }

  return 0;
}


//ADDED
/* Returns the position of the minimum value in the given map */
/* Surrounding the given position pair_t                      */
int move_to_min(uint8_t map[DUNGEON_Y][DUNGEON_X], pair_t pos){
  uint8_t min_dist = 0xFF; //Max value for uint8

  int8_t i, j, min_x, min_y;
  for(i=-1; i<2; i++){
    for(j=-1; j<2; j++){
      if(map[pos[dim_y] + i][pos[dim_x] + j] < min_dist){
        min_dist = map[pos[dim_y] + i][pos[dim_x] + j]; //Update min position

        min_y = i; // Update min value
        min_x = j;
      } else {
        continue;
      }
    }
  }
  pos[dim_y] += min_y;
  pos[dim_x] += min_x;
  return 0;
}

//ADDED
int move_pc(dungeon_t *d, int8_t y, int8_t x){
  switch(d->map[d->pc.position[dim_y] + y][d->pc.position[dim_x] + x]){
    case ter_debug:
    case ter_wall:
    case ter_wall_immutable:
      return 1;
    case ter_floor:
    case ter_floor_room:
    case ter_floor_hall:
      d->pc.position[dim_y] += y;
      d->pc.position[dim_x] += x;
      break;
  }
  return 0;
}

//ADDED
int init_monsters(dungeon_t *d){
  d->num_monsters =0;

  int8_t i;
  for(i=0; i<MAX_MONSTERS; i++){
    int r_room = rand() % d->num_rooms;
    d->monsters[i].position[dim_x] = (d->rooms[r_room].position[dim_x] +
                            (rand() % d->rooms[r_room].size[dim_x]));
    d->monsters[i].position[dim_y] = (d->rooms[r_room].position[dim_y] +
                            (rand() % d->rooms[r_room].size[dim_y]));
    d->monsters[i].attributes = rand() % 15;
    d->num_monsters++;
    if(rand() % 10 < 1) {
      break;
    }
  }
  printf("M:%d\n", d->num_monsters);
  return 0;
}

int main(int argc, char *argv[])
{
  dungeon_t d;
  time_t seed;
  struct timeval tv;
  uint32_t i;
  uint32_t do_load, do_save, do_seed, do_image;
  uint32_t long_arg;
  char *save_file;
  char *pgm_file;

  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = 0;
  do_seed = 1;
  save_file = NULL;

  /* The project spec requires '--load' and '--save'.  It's common  *
   * to have short and long forms of most switches (assuming you    *
   * don't run out of letters).  For now, we've got plenty.  Long   *
   * forms use whole words and take two dashes.  Short forms use an *
`   * abbreviation after a single dash.  We'll add '--rand' (to     *
   * specify a random seed), which will take an argument of it's    *
   * own, and we'll add short forms for all three commands, '-l',   *
   * '-s', and '-r', respectively.  We're also going to allow an    *
   * optional argument to load to allow us to load non-default save *
   * files.  No means to save to non-default locations, however.    *
   * And the final switch, '--image', allows me to create a dungeon *
   * from a PGM image, so that I was able to create those more      *
   * interesting test dungeons for you.                             */
 
 if (argc > 1) {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
      if (argv[i][0] == '-') { /* All switches start with a dash */
        if (argv[i][1] == '-') {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1]) {
        case 'r':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-rand")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        case 'l':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-load"))) {
            usage(argv[0]);
          }
          do_load = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            save_file = argv[++i];
          }
          break;
        case 's':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-save"))) {
            usage(argv[0]);
          }
          do_save = 1;
          break;
        case 'i':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-image"))) {
            usage(argv[0]);
          }
          do_image = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            pgm_file = argv[++i];
          }
          break;
        default:
          usage(argv[0]);
        }
      } else { /* No dash */
        usage(argv[0]);
      }
    }
  }

  if (do_seed) {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  printf("Seed is %ld.\n", seed);
  srand(seed);

  if (do_load) {
    read_dungeon(&d, save_file);
  } else if (do_image) {
    read_pgm(&d, pgm_file);
  } else {
    gen_dungeon(&d);
  }

  i = rand() % d.num_rooms;
  d.pc.position[dim_x] = (d.rooms[i].position[dim_x] +
                          (rand() % d.rooms[i].size[dim_x]));
  d.pc.position[dim_y] = (d.rooms[i].position[dim_y] +
                          (rand() % d.rooms[i].size[dim_y]));
  
  init_monsters(&d);

  int8_t j, k;

  for(j=0; j < 15; j++){
    render_dungeon(&d);

    dijkstra(&d);
    dijkstra_tunnel(&d);
    *d.monsters[0].target = *d.pc.position; //CHANGE
    for(k=0; k<d.num_monsters; k++){
      move_to_min(d.pc_distance, d.monsters[k].position);
    }

    sleep(1);
    move_pc(&d, (rand() % 3) -1, (rand() %3) -1);
  }

  if (do_save) {
    write_dungeon(&d);
  }

  return 0;
}
