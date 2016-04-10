#include <limits.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "dungeon.h"
#include "p_queue.h"
#include "prompt.h"

#define rand_hard rand() % 200 + 50
#define path_hard 1
#define room_hard 2

#define DIR "/.rlg229"
#define SAVE "/dungeon"
#define DES "RLG229"

void print_dungeon(dungeon *d) {
  int i, j;
  for (i = d->Ywin; i < d->Ywin + d->win_height; i++) {
    for (j = d->Xwin; j < d->Xwin + d->win_len; j++) {
      point_type temp = {j, i};
      object_t *obj;
      player_t *ocupant = is_ocupied(d, temp);
      if (get_type(d, j, i) == WALL)
        mvprintw(i - d->Ywin, j - d->Xwin, "#");
      else if (ocupant != NULL)
	      player_print(ocupant, j - d->Xwin, i - d->Ywin);
      else if ( num_objs(d, j, i) != 0){
        obj = peek_objs(d, j, i);
        if(num_objs(d, j, i) > 1){

          attron(COLOR_PAIR(obj->color_num));
	  mvprintw(i - d->Ywin, j - d->Xwin, "&");
          attroff(COLOR_PAIR(obj->color_num));
	}
        else
          obj_print(obj, j - d->Xwin, i - d->Ywin);
      }
      else if (get_type(d, j, i) == ROOM)
        mvprintw(i - d->Ywin, j - d->Xwin, " ");
      else if (get_type(d, j, i) == PATH)
        mvprintw(i - d->Ywin, j - d->Xwin, " ");
      else if (get_type(d, j, i) == DEBUG)
        mvprintw(i - d->Ywin, j - d->Xwin, "*");
      else if (get_type(d, j, i) == STAIRS_UP)
        mvprintw(i - d->Ywin, j - d->Xwin, "<");
      else if (get_type(d, j, i) == STAIRS_DOWN)
        mvprintw(i - d->Ywin, j - d->Xwin, ">");
      else
        mvprintw(i - d->Ywin, j - d->Xwin, ".");
    }
    mvprintw(24, 0, "PC has %d hitpoints and %d items equiped", d->pc->hitpoints, object_list_size(&(d->pc->attr.pc.equiped)));
    refresh();
  }
}

void save_dungeon(dungeon *d) {
  char *home = getenv("HOME");
  char *dir = malloc(strlen(home) + strlen(DIR) + strlen(SAVE) + 1);
  strcpy(dir, home);
  strcat(dir, DIR);

  FILE *file;
  int size;
  int x;
  int y;
  int user_block = 0;
  uint8_t is_room, is_path, is_open, has_stairs;
  uint8_t hardness;

  uint8_t write_8;
  uint16_t write_16;
  uint32_t write_32;

  size = htonl(4 + d->Xmax * d->Ymax * 5 + 2 + 4 + 2 + 4 + 4 + 2 +
               36 * d->num_monsters + user_block);

  //  printf("Saving to %s\n", dir);

  mkdir(dir, S_IRWXU | S_IRWXG | S_IRWXO);

  strcat(dir, SAVE);

  file = fopen(dir, "w");
  free(dir);

  // file marker
  fwrite(DES, 1, strlen(DES), file);
  write_32 = htonl(d->version);

  // file version
  fwrite(&write_32, 1, sizeof(write_32), file);

  // size
  fwrite(&size, 1, sizeof(size), file);

  // userblock size
  write_32 = htonl(user_block);
  fwrite(&write_32, 1, sizeof(write_32), file);

  for (y = 0; y < d->Ymax; y++) {
    for (x = 0; x < d->Xmax; x++) {
      is_room = get_type(d, x, y) == ROOM;
      is_path = get_type(d, x, y) == PATH;
      is_open = is_room || is_path;
      hardness = get(d, x, y);
      if (get_type(d, x, y) == STAIRS_UP)
        has_stairs = 2;
      else if (get_type(d, x, y) == STAIRS_DOWN)
        has_stairs = 1;
      else
        has_stairs = 0;

      // is open
      fwrite(&is_open, 1, sizeof(is_open), file);

      // is_room
      fwrite(&is_room, 1, sizeof(is_room), file);

      // is path
      fwrite(&is_path, 1, sizeof(is_path), file);

      // hardness
      fwrite(&hardness, 1, sizeof(hardness), file);

      // has_stairs
      fwrite(&has_stairs, 1, sizeof(has_stairs), file);
    }
  }
  write_16 = htons(d->num_rooms);
  // number of rooms
  fwrite(&write_16, 1, sizeof(write_16), file);

  for (x = 0; x < d->num_rooms; x++) {

    // X position
    fwrite((char *)&(d->rooms[x].corner[0].x), 1, sizeof(char), file);

    // y position
    fwrite((char *)&(d->rooms[x].corner[0].y), 1, sizeof(char), file);

    // Width
    fwrite((char *)&(d->rooms[x].Xlen), 1, sizeof(char), file);

    // height
    fwrite((char *)&(d->rooms[x].Ylen), 1, sizeof(char), file);
  }

  // PC X position
  fwrite((char *)&(d->pc->position.x), 1, sizeof(char), file);

  // PC Y position
  fwrite((char *)&(d->pc->position.y), 1, sizeof(char), file);

  // PC game turn
  write_32 = htonl(d->pc->turn);
  fwrite(&write_32, 1, sizeof(write_32), file);

  // next npc index
  write_32 = htonl(get_next_index());
  fwrite(&write_32, 1, sizeof(write_32), file);

  // num monsters
  write_16 = htons(d->num_monsters);
  fwrite(&write_16, 1, sizeof(uint16_t), file);

  unsigned char attr;

  for (x = 0; x < d->num_monsters; x++) {
    // symbol (char)
    fwrite((char *)&(d->monsters[x].symbol), 1, sizeof(char), file);

    // x position (unsigned)
    fwrite((unsigned char *)&(d->monsters[x].position.x), 1,
           sizeof(unsigned char), file);

    // y position (unsigned)
    fwrite((unsigned char *)&(d->monsters[x].position.y), 1,
           sizeof(unsigned char), file);

    // speed (unsigned)
    fwrite((unsigned char *)&(d->monsters[x].speed), 1, sizeof(unsigned char),
           file);

    // intel (unsigned)
    attr = has_att(&(d->monsters[x]), SMART);
    fwrite(&attr, 1, sizeof(unsigned char), file);

    // tele (unsigned)
    attr = has_att(&(d->monsters[x]), TELE);
    fwrite(&attr, 1, sizeof(unsigned char), file);

    // last_known x (unsigned), 255 if NA
    fwrite((unsigned char *)&(d->monsters[x].attr.npc.last_seen.x), 1,
           sizeof(unsigned char), file);

    // last_ known y (unsigned), 255 if N
    fwrite((unsigned char *)&(d->monsters[x].attr.npc.last_seen.y), 1,
           sizeof(unsigned char), file);

    // index (4 bytes)
    write_32 = htonl(d->monsters[x].index);
    fwrite(&write_32, 1, sizeof(uint32_t), file);

    // turn (4 bytes)
    write_32 = htonl(d->monsters[x].turn);
    fwrite(&write_32, 1, sizeof(uint32_t), file);

    // is_dead
    write_8 = has_att(&(d->monsters[x]), DEAD);
    fwrite(&write_8, 1, sizeof(write_8), file);

    // is_seen
    write_8 = has_att(&(d->monsters[x]), SEEN);
    fwrite(&write_8, 1, sizeof(write_8), file);

    unsigned char extra = 0;

    // 18 extra bytes, 0 if not used
    for (y = 0; y < 18; y++) {
      fwrite(&extra, 1, sizeof(unsigned char), file);
    }
  }

  fclose(file);
}

int load_dungeon(dungeon *d, player_t *pc) {
  char *home = getenv("HOME");
  char *dir = malloc(strlen(home) + strlen(DIR) + strlen(SAVE) + 1);
  strcpy(dir, home);
  strcat(dir, DIR);
  strcat(dir, SAVE);
  FILE *file;
  char *descripter = malloc(strlen(DES) + 1);

  int size;
  int user_offset;
  int x, y;
  int8_t is_room, is_path, is_open, has_stairs;
  uint8_t hardness;

  uint8_t read_8;
  uint16_t read_16;
  uint32_t read_32;

  file = fopen(dir, "r");

  free(dir);

  if (file == NULL) {
    free(descripter);
    return -1;
  }

  x = fread(descripter, 1, strlen(DES), file);
  descripter[x] = '\0';

  if (strcmp(descripter, DES))
    return -1;

  free(descripter);

  fread(&read_32, 1, sizeof(read_32), file);
  d->version = ntohl(read_32);
  fprintf(stderr, "Version %d.\n\r", d->version);

  fread(&read_32, 1, sizeof(read_32), file);
  size = ntohl(read_32);

  fprintf(stderr, "Size: %d\n", read_32);

  fread(&read_32, 1, sizeof(read_32), file);
  user_offset = ntohl(read_32);
  fprintf(stderr, "user offset is: %d\n\r", user_offset);

  //  if(d->version == 0){
  d->Xmax = 160;
  d->Ymax = 96;
  d->area = d->Xmax * d->Ymax;
  d->pc = pc;
  d->grid = malloc(sizeof(*d->grid) * d->area);
  fprintf(stderr, "Xmax = %d\nYmax = %d\narea = %d\n", d->Xmax, d->Ymax,
          d->area);
  //  }
  // else
  //  return -1;

  for (y = 0; y < d->Ymax; y++) {
    for (x = 0; x < d->Xmax; x++) {
      fread(&is_open, 1, sizeof(is_open), file);
      fread(&is_room, 1, sizeof(is_room), file);
      fread(&is_path, 1, sizeof(is_path), file);
      fread(&hardness, 1, sizeof(hardness), file);
      fread(&has_stairs, 1, sizeof(has_stairs), file);
      if (has_stairs == 1)
        place_type(d, x, y, STAIRS_DOWN);
      else if (has_stairs == 2)
        place_type(d, x, y, STAIRS_UP);
      else if (hardness == 0xff)
        place_type(d, x, y, WALL);
      else if (is_room)
        place_type(d, x, y, ROOM);
      else if (is_path)
        place_type(d, x, y, PATH);
      else {
        place(d, x, y, hardness);
        // Probably should do something different...
        d->grid[d->Ymax * x + y].type = STONE;
      }
      fprintf(stderr, "is_room:%d\nis_path:%d\nstairs:%d\nhardness:%d\n\r",
              is_room, is_path, has_stairs, hardness);
      d->grid[d->Ymax * x + y].ocupant = NULL;
    }
  }

  fprintf(stderr, "Read grid data successfully, %d grids\n\r", x * y);

  fread(&read_16, 1, sizeof(read_16), file);
  d->num_rooms = ntohs(read_16);
  d->rooms = malloc(sizeof(*d->rooms) * d->num_rooms);
  for (x = 0; x < d->num_rooms; x++) {
    uint8_t temp;
    fread(&read_8, 1, sizeof(read_8), file);
    temp = read_8;
    fread(&read_8, 1, sizeof(read_8), file);
    set_coord(&d->rooms[x], (int)temp, (int)read_8);

    fread(&read_8, 1, sizeof(read_8), file);
    temp = read_8;
    fread(&read_8, 1, sizeof(read_8), file);
    d->rooms[x].Xlen = (int)temp;
    d->rooms[x].Ylen = (int)read_8;
    d->rooms[x].room_id = x;
    d->rooms[x].area = d->rooms[x].Xlen * d->rooms[x].Ylen;
  }

  fprintf(stderr, "read room data successfully, %d rooms\n\r", x - 1);

  // PC position
  fread(&read_8, 1, sizeof(read_8), file);
  d->pc->position.x = read_8;
  fread(&read_8, 1, sizeof(read_8), file);
  d->pc->position.y = read_8;

  d->grid[d->Ymax * d->pc->position.x + d->pc->position.y].ocupant = d->pc;

  // game turn
  fread(&read_32, 1, sizeof(read_32), file);
  set_turn(d->pc, ntohl(read_32));

  fprintf(stderr, "next game turn is %d\n\r", read_32);

  fread(&read_32, 1, sizeof(read_32), file);
  set_next_index(ntohl(read_32));

  fprintf(stderr, "next generation index is %d\n\r", read_32);

  fread(&read_16, 1, sizeof(read_16), file);
  d->num_monsters = ntohs(read_16);

  fprintf(stderr, "number of monsters is %d\n\r", d->num_monsters);

  d->num_monsters_alive = d->num_monsters;
  d->monsters = malloc(sizeof(player_t) * d->num_monsters);

  for (x = 0; x < d->num_monsters; x++) {
    // Allocate memory for Name
    d->monsters[x].attr.npc.name = malloc(sizeof(char) * 1);
    d->monsters[x].attr.npc.name[0] = 0;

    // Allocate memory for Description
    d->monsters[x].attr.npc.description = malloc(sizeof(char) * 1);
    d->monsters[x].attr.npc.description[0] = 0;

    // symbol (byte)
    fread(&read_8, 1, sizeof(read_8), file);
    d->monsters[x].symbol = read_8;

    // x (unsinged)
    fread(&read_8, 1, sizeof(read_8), file);
    d->monsters[x].position.x = (int)read_8;

    // y (unsigned)
    fread(&read_8, 1, sizeof(read_8), file);
    d->monsters[x].position.y = (int)read_8;

    d->grid[d->Ymax * x + y].ocupant = &(d->monsters[x]);

    // speed (unsigned)
    fread(&read_8, 1, sizeof(read_8), file);
    d->monsters[x].speed = (int)read_8;

    // intel (unsigned)
    fread(&read_8, 1, sizeof(read_8), file);
    if (read_8)
      set_att(&(d->monsters[x]), SMART);

    // tele  (unsigned)
    fread(&read_8, 1, sizeof(read_8), file);
    if (read_8)
      set_att(&(d->monsters[x]), TELE);

    // last_known x (unsigned)
    fread(&read_8, 1, sizeof(read_8), file);
    d->monsters[x].attr.npc.last_seen.x = (int)read_8;

    // last_known y (unsigned)
    fread(&read_8, 1, sizeof(read_8), file);
    d->monsters[x].attr.npc.last_seen.y = (int)read_8;

    // index (4 bytes)
    fread(&read_32, 1, sizeof(read_32), file);
    set_index(&(d->monsters[x]), ntohl(read_32));

    // next_turn (4 bytes)
    fread(&read_32, 1, sizeof(read_32), file);
    set_turn(&(d->monsters[x]), ntohl(read_32));

    // is_dead
    fread(&read_8, 1, sizeof(read_8), file);
    if (read_8)
      set_att(&(d->monsters[x]), DEAD);

    // is_seen
    fread(&read_8, 1, sizeof(read_8), file);
    if (read_8)
      set_att(&(d->monsters[x]), SEEN);

    // extra
    for (y = 0; y < 18; y++) {

      fread(&read_8, 1, sizeof(unsigned char), file);
    }
  }

  center_on_pc(d);

  fclose(file);
  if (size > 0)
    return 0;
  else
    return -1;
}

int generate_dungeon(dungeon *d, player_t *pc, int num_rooms, int num_monsters,
                     void *monster_templates, int num_objects, void *object_templates, int Xmax, int Ymax) {
  // Set the X and Y dimensions of dungeon
  d->Xmax = Xmax;
  d->Ymax = Ymax;
  d->area = d->Xmax * d->Ymax;
  
  // Set window sizes
  d->win_len = WIN_LEN;
  d->win_height = WIN_HEIGHT;

  // Set number of rooms, monsters, and objects
  d->num_rooms = num_rooms;
  d->num_monsters = num_monsters;
  d->num_objects = num_objects;
  
  // Number of monsters alive in dungeon
  d->num_monsters_alive = num_monsters;
  
  // Set PC - will be reset in main function.....
  d->pc = pc;
  
  // Set dungeon version 
  d->version = 1;

  // Allocate memory for Dungeon and Rooms
  d->grid = malloc(sizeof(*d->grid) * d->area);
  d->rooms = malloc(sizeof(*d->rooms) * num_rooms);

  // Center board_view on PC
  if (d->grid == 0 || d->rooms == 0)
    return -1;
  if (d->win_len > d->Xmax || d->win_height > d->Ymax) {
    printf("Please redfine your window dimensions, ensure that they are at "
           "least the size of your dungeon.\n");
    return -1;
  }

  // Set boarders to WALL and rest of grid to STONE
  // All spaces have no ocupant
  int i, j;
  for (i = 0; i < d->Xmax; i++) {
    for (j = 0; j < d->Ymax; j++) {
      if (i % (d->Xmax - 1) == 0 || j % (d->Ymax - 1) == 0)
        place_type(d, i, j, WALL);
      else
        place_type(d, i, j, STONE);
      place_ocupant(d, i, j, NULL);
      place_object(d, i, j, NULL);
    }
  }

  // Generate rooms - place into rooms array
  if (populate_rooms(d) == -1) {
    printf("Problem populating rooms - restarting.\n");
    return -2;
  }

  // Place rooms in dungeon
  if (place_rooms(d) == -1) {
    printf("Problem placing rooms - restarting.\n");
    return -2;
  }

  // Carve paths between rooms - ?
  finalize_rooms(d);

  if (connect_rooms(d) == -1) {
    printf("Problem connecting rooms - restarting.\n");
    return -2;
  }

  if (place_character(d, pc) == -1) {
    printf("Problem placing PC - restarting.\n");
    return -2;
  }


  // Generate monsters
  if(generate_monsters(&(d->monsters), monster_templates, d->num_monsters) == -1){
    printf("Problem generating monsters - restarting.\n");
    return -2;
  }

  if (place_monsters(d) == -1) {
    printf("Problem placing monsters - restarting.\n");
    return -2;
  }
  
  // Generate Objects
  if( generate_objects(&(d->objects), object_templates, d->num_objects) == -1){
    printf("Problem generating objects - restarting.\n");
    return -2;
  }
  
  // Place Objects
  if( place_objects(d) == -1){
    printf("Problem placing objects - restarting.\n");
    return -2;
  }

  for (i = 0; i < d->num_objects; i++)
    destroy_obj(&(d->objects[i]));
  free(d->objects);
  
  return 0;
}

int place_objects(dungeon *d){
  int i, x, y, tries;
  int max_tries = 100;
  for(i = 0; i < d->num_objects; i++){
    tries = 0;
    do{
      x = (rand() % ((d->Xmax)-3) )+1;
      y = (rand() % ((d->Ymax)-3) )+1;
      tries++;
    }while( place_object(d, x, y, &(d->objects[i])) == -1 && tries < max_tries);
  }
  if(tries >= max_tries)
    return -1;
  return 0;
}

int num_objs(dungeon *d, int x, int y){
  return obj_stack_size( &(d->grid[d->Ymax * x + y].objs ));
}

int place_object(dungeon *d, int x, int y, object_t *obj){
  if(obj == NULL){
    obj_stack_init(&(d->grid[d->Ymax * x + y].objs), sizeof(object_t));
  }
  else if((d->grid[d->Ymax * x + y].type == ROOM || d->grid[d->Ymax * x + y].type == PATH) && d->grid[d->Ymax * x + y].type != STAIRS_UP && d->grid[d->Ymax * x + y].type != STAIRS_DOWN){
    obj_stack_push(&(d->grid[d->Ymax * x + y].objs), obj);
    return 0;
  }
  return -1;
}

int pop_objs(dungeon *d, int x, int y, object_t *obj){
  if(obj_stack_size(&(d->grid[d->Ymax * x + y].objs)) > 0){
    obj_stack_pop(&(d->grid[d->Ymax * x + y].objs), obj);
    return 0;
  }
  else
  return -1;
}

object_t *peek_objs(dungeon *d, int x, int y){
 if(obj_stack_size(&(d->grid[d->Ymax * x + y].objs)) > 0){
  return obj_stack_peep(&d->grid[d->Ymax * x + y].objs);
  }else
    return NULL;
}

int place_monsters(dungeon *d) {
  int i;
  for (i = 0; i < d->num_monsters; i++) {
    if (place_character(d, &(d->monsters[i])) == -1)
      return -1;
  }
  return 0;
}

int place_character(dungeon *d, player_t *player) {
  int room, try
    = 0, max_tries = 50;
  point_type temp;
  do {
    room = rand() % (d->num_rooms);
    temp.x = d->rooms[room].corner[0].x + rand() % (d->rooms[room].Xlen);
    temp.y = d->rooms[room].corner[0].y + rand() % (d->rooms[room].Ylen);
  } while (is_ocupied(d, temp) != NULL && try ++ < max_tries);
  if (try >= max_tries)
    return -1;
  place_ocupant(d, temp.x, temp.y, player);
  if (player->index == 0) {
    center_on_pc(d);
  }
  return 0;
}

void center_on_pc(dungeon *d) {
  d->Xwin = (d->pc->position.x - (d->win_len / 2) >= 0)
                ? (d->pc->position.x - (d->win_len / 2))
                : 0;
  d->Ywin = (d->pc->position.y - (d->win_height / 2) >= 0)
                ? (d->pc->position.y - (d->win_height / 2))
                : 0;

  if (d->Xwin + d->win_len > d->Xmax)
    d->Xwin = d->Xwin - (d->Xwin + d->win_len - d->Xmax);

  if (d->Ywin + d->win_height > d->Ymax)
    d->Ywin = d->Ywin - (d->Ywin + d->win_height - d->Ymax);
  print_dungeon(d);
}

void look_around(dungeon *d) {
  int input;
  do {
    input = getch();
    switch (input) {
    case (27):
      return;
      break;
    case ('6'):
    case ('l'):
    case (KEY_RIGHT):
      d->Xwin = (d->Xwin + d->win_len < d->Xmax) ? d->Xwin + 1 : d->Xwin;
      break;
    case ('2'):
    case ('j'):
    case (KEY_DOWN):
      d->Ywin = (d->Ywin + d->win_height < d->Ymax) ? d->Ywin + 1 : d->Ywin;
      break;
    case ('4'):
    case ('h'):
    case (KEY_LEFT):
      d->Xwin = (d->Xwin - 1 >= 0) ? d->Xwin - 1 : d->Xwin;
      break;
    case ('8'):
    case ('k'):
    case (KEY_UP):
      d->Ywin = (d->Ywin - 1 >= 0) ? d->Ywin - 1 : d->Ywin;
      break;
    default:
      continue;
    }
    print_dungeon(d);
  } while (1);
}

player_t *is_ocupied(dungeon *d, point_type location) {
  return d->grid[d->Ymax * location.x + location.y].ocupant;
}

void place_ocupant(dungeon *d, int x, int y, player_t *player) {
  d->grid[d->Ymax * x + y].ocupant = player;
  if (player == NULL)
    return;
  player->position.x = x;
  player->position.y = y;
  if (player->index != 0) {
    last_seen(d, player);
  }
}

int same_room(dungeon *d, point_type A, point_type B) {
  if (get_type(d, A.x, A.y) == ROOM && get_type(d, B.x, B.y) == ROOM) {
    if (d->grid[A.x * d->Ymax + A.y].label ==
        d->grid[B.x * d->Ymax + B.y].label)
      return 0;
  }
  return 1;
}

int populate_rooms(dungeon *d) {
  int i, area, area_round;
  int used_area = 0;
  int tries_allowed = 2000;
  for (i = 0; i < d->num_rooms; i++) {
    int try
      = 0;

    do {
      area_round = (((d->Xmax) * (d->Ymax) - used_area) / 2 / d->num_rooms);
      area = rand() % (area_round - 4 * 8) + 4 * 8;
      used_area += area;
      try
        ++;
    } while (generate_room(&(d->rooms[i]), 4, 8, area) == 1 &&
             try < tries_allowed);

    if (try > tries_allowed)
      return -1;
  }

  return 0;
}

int safe_location(dungeon *d, int x, int y, int room_id) {
  int i;
  int hd_offset = 3;
  if (x > 0 && x + d->rooms[room_id].Xlen < (d->Xmax) - 1 && y > 0 &&
      y + d->rooms[room_id].Ylen < (d->Ymax) - 1) {
    for (i = 0; i < room_id; i++) {
      if (x + d->rooms[room_id].Xlen + hd_offset < d->rooms[i].corner[0].x ||
          d->rooms[i].corner[2].x + hd_offset < x)
        continue;
      else if (y + d->rooms[room_id].Ylen + hd_offset <
                   d->rooms[i].corner[0].y ||
               d->rooms[i].corner[2].y + hd_offset < y)
        continue;
      else
        return 1;
    }
    return 0;
  }
  return 1;
}

int place_rooms(dungeon *d) {
  int i, j, k, x, y, tries = 0;
  for (i = 0; i < d->num_rooms; i++) {
    do {
      x = rand() % (d->Xmax - d->rooms[i].Xlen - 2) + 1;
      y = rand() % (d->Ymax - d->rooms[i].Ylen - 2) + 1;
      tries++;
    } while (safe_location(d, x, y, i) == 1 && tries < 1000);

    if (tries < 1000) {
      set_coord(&d->rooms[i], x, y);
      for (j = 0; j < d->rooms[i].Xlen; j++) {
        for (k = 0; k < d->rooms[i].Ylen; k++) {
          d->grid[(x + j) * d->Ymax + (y + k)].label = i;
        }
      }
    } else
      return -1;
  }

  return 0;
}

void finalize_rooms(dungeon *d) {
  int i;
  for (i = 0; i < d->num_rooms; i++) {
    int x;
    int y;
    for (x = d->rooms[i].corner[0].x; x < d->rooms[i].corner[2].x; x++) {
      for (y = d->rooms[i].corner[0].y; y < d->rooms[i].corner[2].y; y++) {
        place_type(d, x, y, ROOM);
      }
    }
    if (rand() % 2) {
      x = rand() % d->rooms[i].Xlen + d->rooms[i].corner[0].x;
      y = rand() % d->rooms[i].Ylen + d->rooms[i].corner[0].y;
      place_type(d, x, y, STAIRS_UP + rand() % 2);
    }
  }
}

int connect_rooms(dungeon *d) {
  int i, j, num_con = 0, distance;
  for (i = 0; i < d->num_rooms; i++) {
    int shortest_distance = INT_MAX;
    int closest_room = -1;
    point_type center_a, center_b;
    find_center(&d->rooms[i], &center_a);
    for (j = 0; j < num_con; j++) {
      find_center(&d->rooms[j], &center_b);
      distance = distance_between(center_a, center_b);
      if (shortest_distance > distance) {
        shortest_distance = distance;
        closest_room = j;
      }
    }
    if (closest_room != -1) {
      if (carve_path(d, i, closest_room) == -1)
        return -1;
    }
    num_con++;
  }
  return 0;
}

// Need function that retuns the 8 surrounding ponts of given point
void neighbor(point_type source, point_type *neighbors) {
  // int deltas[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1},
  // {-1, 0}, {-1, 1}};
  int deltas[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  int i;
  for (i = 0; i < 4; i++) {
    neighbors[i].x = source.x + deltas[i][0];
    neighbors[i].y = source.y + deltas[i][1];
  }
}

int carve_path(dungeon *d, int start_room, int end_room) {
  point_type start;
  point_type end;

  int top_bottom = rand() % 2;
  // Find facing sides, random top or bottom
  if (top_bottom) {
    // going off of random location in sides
    if (d->rooms[start_room].corner[2].x < d->rooms[end_room].corner[0].x) {
      // start is left of end
      start.x = d->rooms[start_room].corner[2].x;
      end.x = d->rooms[end_room].corner[0].x;
    } else {
      // start is right of end
      start.x = d->rooms[start_room].corner[0].x;
      end.x = d->rooms[end_room].corner[2].x;
    }

    start.y =
        rand() % (d->rooms[start_room].Ylen) + d->rooms[start_room].corner[0].y;
    end.y = rand() % (d->rooms[end_room].Ylen) + d->rooms[end_room].corner[0].y;

  } else {
    // going off of random location in top
    if (d->rooms[start_room].corner[2].y < d->rooms[end_room].corner[0].y) {
      // start is above end
      start.y = d->rooms[start_room].corner[2].y;
      end.y = d->rooms[end_room].corner[0].y;
    } else {
      // start is below end
      start.y = d->rooms[start_room].corner[0].y;
      end.y = d->rooms[end_room].corner[2].y;
    }
    start.x =
        rand() % (d->rooms[start_room].Xlen) + d->rooms[start_room].corner[0].x;
    end.x = rand() % (d->rooms[end_room].Xlen) + d->rooms[end_room].corner[0].x;
  }

  find_center(&d->rooms[start_room], &start);
  find_center(&d->rooms[end_room], &end);

  print_path(d, start, end);
  return 0;
}

void print_path(dungeon *d, point_type start, point_type end) {
  point_type parent[d->Xmax * d->Ymax];
  int old_x;
  dij(d, start, end, parent, 0);
  do {
    if (get_type(d, end.x, end.y) == STONE)
      place_type(d, end.x, end.y, PATH);
    old_x = end.x;
    end.x = parent[old_x * d->Ymax + end.y].x;
    end.y = parent[old_x * d->Ymax + end.y].y;
  } while (end.x != INT_MAX && end.y != INT_MAX);
}

int last_seen(dungeon *d, player_t *p) {
  
  // If monster is in the same room as PC
  if (same_room(d, d->pc->position, p->position) == 0) {
    // Update monster so knows the last seen position of PC
    p->attr.npc.last_seen = d->pc->position;
    // Set monster's seen attribute.
    set_att(p, SEEN);

    return 0;
  } 

  // Else, monster is not in the same room as PC
  // Now we will check if can get a line-of-site
  else {
    
    // Used for calculating direction offset.
    int delta[2] = {1, -1};
    int i;
    point_type temp;
    player_t *temp_player;
    
    for (i = 0; i < 2; i++) {
      temp.x = p->position.x;
      temp.y = p->position.y;
      do {
        temp.x += delta[i];
      } while (get_type(d, temp.x, temp.y) == PATH &&
               is_ocupied(d, temp) == NULL);
      temp_player = is_ocupied(d, temp);
      
      
      if (temp_player == d->pc) {
        p->attr.npc.last_seen = temp;
        set_att(p, SEEN);
        return 0;
      } 

      else {
        temp.x = p->position.x;
        temp.y = p->position.y;
        do {
          temp.y += delta[i];
        } while (get_type(d, temp.x, temp.y) == PATH &&
                 is_ocupied(d, temp) == NULL);
        temp_player = is_ocupied(d, temp);
        if (temp_player == d->pc) {
          p->attr.npc.last_seen = temp;
          set_att(p, SEEN);
          // printf("%x sees @, in a path\n", p->index);
          return 0;
        }
      }
    }
    return 1;
  }
}

int move_ocupant(dungeon *d, player_t *p) {
  point_type start, end, next;
  next.x = 0;
  next.y = 0;
  int x, y;
  int input;

  // Current position of the ocupant
  start = p->position;

  // If monster, need to find destination
  if (p->index != 0) {
    
    //update last_seen field - will be 
    last_seen(d, p);
    if (has_att(p, TELE)) {
      end = d->pc->position;
    } else {
      end = p->attr.npc.last_seen;
    }

    if (has_att(p, SMART)) {
      if (has_att(p, TELE) || has_att(p, SEEN)) {
        smart_next_move(d, start, end, &next);
        if (has_att(p, SEEN) && end.x == next.x && end.y == next.y && !(next.x == d->pc->position.x && next.y == d->pc->position.y)) {
          reset_att(p, SEEN);
          return 0;
        }
      } else {
        meander(d, start, &next);
      }
    } else {
      if (has_att(p, TELE) || has_att(p, SEEN)) {
        dumb_next_move(d, start, end, &next);
      } else {
        meander(d, start, &next);
      }
    }
  } else {
    //    meander(d, start, &next);
    int no_op = 1;
    while (no_op) {
      no_op = 0;
      input = getch();
      switch (input) {
      // upper-left
      case ('7'):
      case ('y'):
        x = -1;
        y = -1;
        break;
      case ('8'):
      case ('k'):
      case (KEY_UP):
        // up
        x = 0;
        y = -1;
        break;
      case ('9'):
      case ('u'):
        // upper-right
        x = 1;
        y = -1;
        break;
      case ('6'):
      case ('l'):
      case (KEY_RIGHT):
        // right
        x = 1;
        y = 0;
        break;
      case ('3'):
      case ('n'):
        // lower-right
        x = 1;
        y = 1;
        break;
      case ('2'):
      case ('j'):
      case (KEY_DOWN):
        // down
        x = 0;
        y = 1;
        break;
      case ('1'):
      case ('b'):
        // lower-left
        x = -1;
        y = 1;
        break;
      case ('4'):
      case ('h'):
      case (KEY_LEFT):
        // left
        x = -1;
        y = 0;
        break;
	// save and exit
      case ('S'):
        return 3;
        break;
	// look-around
      case ('L'):
        look_around(d);
        center_on_pc(d);
        //return 0;
	no_op = 1;
	break;
	// skip turn
      case (' '):
        x = 0;
        y = 0;
        break;
      case ('<'):
        if (get_type(d, p->position.x, p->position.y) == STAIRS_UP) {
          return 2;
        }
	break;
      case ('>'):
        if (get_type(d, p->position.x, p->position.y) == STAIRS_DOWN) {
          return 2;
        }
	break;

	//Options for PC object menu
      case ('w'):
	pc_item_equip(d);
	no_op = 1;
	break;
      case ('d'):
	pc_item_drop(d);
	no_op = 1;
	break;
      case ('t'):
	pc_item_unequip(d);
	no_op = 1;
	break;
      case ('x'):
	pc_item_delete(d);
	no_op = 1;
	break;

      default:
        // return 0;
        no_op = 1;
        break;
      }
    }
    if (get_type(d, start.x + x, start.y + y) != WALL &&
        get_type(d, start.x + x, start.y + y) != STONE) {
      next.x = start.x + x;
      next.y = start.y + y;

      if (next.x < d->Xwin + (d->win_len / 4))
        d->Xwin = (d->Xwin - 1 >= 0) ? d->Xwin - 1 : d->Xwin;

      if (next.x > (d->Xwin + d->win_len) - (d->win_len / 4))
        d->Xwin = (d->Xwin + d->win_len < d->Xmax) ? d->Xwin + 1 : d->Xwin;

      if (next.y < d->Ywin + (d->win_height / 4))
        d->Ywin = (d->Ywin - 1 >= 0) ? d->Ywin - 1 : d->Ywin;

      if (next.y > (d->Ywin + d->win_height) - (d->win_height / 4))
        d->Ywin = (d->Ywin + d->win_height < d->Ymax) ? d->Ywin + 1 : d->Ywin;
    } else {
      next.x = start.x;
      next.y = start.y;
    }
  }

  if (d->num_monsters_alive == 0) {
    printf("PC wins!!\n");
    return 1;
  }
 
  // If monster attacks monster
 if (is_ocupied(d, next) != NULL && is_ocupied(d, next) != d->pc &&
      p->index != 0 && is_ocupied(d, next) != p) {
     int deltas[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1},
     {-1, 0}, {-1, 1}};
    player_t *to_move = is_ocupied(d, next);
    //set_att(is_ocupied(d, next), DEAD);
    //d->num_monsters_alive--;
    point_type temp_next;
    int i;
    for(i = 0; i < 8; i++){
      temp_next.x = deltas[i][0] + next.x;
      temp_next.y = deltas[i][1] + next.y;
      if(is_ocupied(d, temp_next) == NULL && get_type(d, temp_next.x, temp_next.y) != WALL && get_type(d, temp_next.x, temp_next.y) != STONE){
        d->grid[temp_next.x * d->Ymax + temp_next.y].ocupant = to_move;
        to_move->position.x = temp_next.x;
        to_move->position.y = temp_next.y;
        d->grid[next.x * d->Ymax + next.y].ocupant = p;
        p->position.x = next.x;
        p->position.y = next.y;
        d->grid[start.x * d->Ymax + start.y].ocupant = NULL;
        break;
      }
    }
    //d->grid[next.x * d->Ymax + next.y].ocupant = NULL;
    //     printf("%c took a monster\nTELE: %d, SMART:%d\n", p->symbol,
    //     has_att(p, TELEPATH), has_att(p, SMART));
    return 0;
  }

  // if PC and monster come under attack
  if (is_ocupied(d, next) != NULL && ((is_ocupied(d, next) == d->pc && p->index != 0 ) || (p->index == 0 && is_ocupied(d, next) != d->pc))) {
    player_t *monster = is_ocupied(d, next) == d->pc ? p : is_ocupied(d, next);
    //int pc_before = d->pc->hitpoints;
    //int mon_before = monster->hitpoints;
    int battle_result = player_attack(d->pc, monster);
    //mvprintw(30, 0, "Battle between PC and %s.", monster->attr.npc.name);
    //mvprintw(31, 0, "%s has %d-%db%d for damage and %d hitpoints", monster->attr.npc.name, monster->damage.base, monster->damage.dice, monster->damage.sides, monster->hitpoints);
    //mvprintw(32, 0, "PC had %d hitpoints and lost %d hitpoints", pc_before, pc_before - d->pc->hitpoints);
    //mvprintw(33, 0, "%s has lost %d hitpoints", monster->attr.npc.name, mon_before - monster->hitpoints);
    //refresh();
//getchar();
    switch(battle_result){
      // Battle Stalemate
      case 0:
        
        //No update, but player's hitpoints have been updated.
        return 0;

      // PC defeats monster
      case 1:
        set_att(is_ocupied(d, next), DEAD);
        d->pc->position = next;
        d->grid[start.x * d->Ymax + start.y].ocupant = NULL;
        d->grid[next.x * d->Ymax + next.y].ocupant = d->pc;
        d->num_monsters_alive--;
        return 0;

      // Monster wins
      case 2:
        //monster->position = next;
        //d->grid[start.x * d->Ymax + start.y].ocupant = NULL;
        //d->grid[next.x * d->Ymax + next.y].ocupant = monster;
        print_dungeon(d);
        return 1;
    }
    return 0;
  }

  // if PC steps on square with item
  if(p->index == 0){
    if(num_objs(d, next.x, next.y) != 0){
      object_t temp;
      pop_objs(d, next.x, next.y, &temp);
      if(player_pickup(d->pc, &temp) != 0){
        place_object(d, next.x, next.y, &temp);
      }
      destroy_obj(&temp);
    }
  }
 

  // update player location
  p->position = next;

  // update board info
  d->grid[start.x * d->Ymax + start.y].ocupant = NULL;
  d->grid[next.x * d->Ymax + next.y].ocupant = p;

  return 0;
}

int dumb_next_move(dungeon *d, point_type start, point_type end, point_type *next) {

  int xdiff = start.x - end.x;
  int ydiff = start.y - end.y;
  // see which distance is furtherst
  if (abs(xdiff) > abs(ydiff)) {
    if (xdiff > 0)
      next->x = start.x - 1;
    else
      next->x = start.x + 1;
    next->y = start.y;
  } else {
    if (ydiff > 0)
      next->y = start.y - 1;
    else
      next->y = start.y + 1;
    next->x = start.x;
  }
  // check that desired next space is valid, not checking for other characters
  if (!(next->x >= 0 && next->x < d->Xmax && next->y >= 0 &&
        next->y < d->Ymax && get_type(d, next->x, next->y) != STONE &&
        get_type(d, next->x, next->y) != WALL)) {
    next->x = start.x;
    next->y = start.y;
  }
  return 0;
}

int smart_next_move(dungeon *d, point_type start, point_type end, point_type *next) {
  point_type parent[d->Xmax * d->Ymax];
  int old_x, old_y;
  dij(d, start, end, parent, 1);
  do {
    old_x = end.x;
    old_y = end.y;
    end.x = parent[old_x * d->Ymax + old_y].x;
    end.y = parent[old_x * d->Ymax + old_y].y;
  } while (!(parent[old_x * d->Ymax + old_y].x == start.x &&
             parent[old_x * d->Ymax + old_y].y == start.y));
  /* if (!(next->x >= 0 && next->x < d->Xmax && next->y >= 0 &&
        next->y < d->Ymax && get_type(d, next->x, next->y) != STONE &&
        get_type(d, next->x, next->y) != WALL)) {
    next->x = old_x;
    next->y = old_y;
     } else {
    next->x = start.x;
    next->y = start.y;
    }*/
    next->x = old_x;
    next->y = old_y;
  return 0;
}

int meander(dungeon *d, point_type start, point_type *next) {
  int x, y;
  int random = rand() % 2;
  do {
    if (random) {
      x = rand() % 3 - 1;
      y = 0;
    } else {
      y = rand() % 3 - 1;
      x = 0;
    }
  } while (get_type(d, start.x + x, start.y + y) == WALL ||
           get_type(d, start.x + x, start.y + y) == STONE);
  next->x = start.x + x;
  next->y = start.y + y;
  return 0;
}

void debug_next_move(dungeon *d) {
  int a, b;
  a = rand() % d->num_rooms;
  do {
    b = rand() % d->num_rooms;
  } while (a == b);
  room *room_a = &d->rooms[a];
  room *room_b = &d->rooms[b];
  point_type start;
  start.x = room_a->corner[0].x + rand() % room_a->Xlen;
  start.y = room_a->corner[0].y + rand() % room_a->Ylen;
  point_type end;
  end.x = room_b->corner[0].x + rand() % room_b->Xlen;
  end.y = room_b->corner[0].y + rand() % room_b->Ylen;
  point_type next;
  place_type(d, start.x, start.y, STONE);
  place_type(d, end.x, end.y, STONE);

  do {
    smart_next_move(d, start, end, &next);
    place_type(d, next.x, next.y, DEBUG);
    start.x = next.x;
    start.y = next.y;
  } while (start.x != end.x || start.y != end.y);
}

void dij(dungeon *d, point_type start, point_type end, point_type *parent, int fast) {
  point_type neighbors[8];
  int weight[d->Xmax][d->Ymax];
  point_type current, temp;
  p_queue q;
  int i;

  int x, y;
  for (x = 0; x < d->Xmax; x++) {
    for (y = 0; y < d->Ymax; y++) {
      weight[x][y] = INT_MAX;
      parent[x * d->Ymax + y].x = INT_MAX;
      parent[x * d->Ymax + y].y = INT_MAX;
    }
  }

  init_q(&q, sizeof(point_type));

  weight[start.x][start.y] = 0;

  push(&q, &start, 0);

  while (pop(&q, &current) == 0) {
    neighbor(current, neighbors);
    for (i = 0; i < 4; i++) {
      if (neighbors[i].x > 0 && neighbors[i].x < d->Xmax &&
          neighbors[i].y > 0 && neighbors[i].y < d->Ymax) {
        if (weight[current.x][current.y] +
                get(d, neighbors[i].x, neighbors[i].y) + 1 <
            weight[neighbors[i].x][neighbors[i].y]) {
          // UPDATE WEIGHT
          weight[neighbors[i].x][neighbors[i].y] =
              weight[current.x][current.y] +
              get(d, neighbors[i].x, neighbors[i].y);
          parent[neighbors[i].x * d->Ymax + neighbors[i].y].x = current.x;
          parent[neighbors[i].x * d->Ymax + neighbors[i].y].y = current.y;
          temp.x = neighbors[i].x;
          temp.y = neighbors[i].y;

          // Break if found ending space
          if (fast == 1 && temp.x == end.x && temp.y == end.y) {
            destroy(&q);
            return;
          }
          push(&q, &temp, weight[neighbors[i].x][neighbors[i].y]);
        }
      }
    }
  }
  destroy(&q);
}

void place(dungeon *d, int x, int y, unsigned char thing) {
  d->grid[d->Ymax * x + y].hardness = thing;
}

void place_type(dungeon *d, int x, int y, floor_type type) {
  d->grid[d->Ymax * x + y].type = type;
  switch (type) {
  case ROOM:
    place(d, x, y, room_hard);
    break;
  case PATH:
    place(d, x, y, path_hard);
    break;
  case WALL:
    place(d, x, y, 255);
    break;
  case STONE:
    place(d, x, y, rand_hard);
    break;
  case STAIRS_UP:
    place(d, x, y, 255);
    break;
  case STAIRS_DOWN:
    place(d, x, y, 255);
    break;
  case DEBUG:
    break;
  }
}

unsigned char get(dungeon *d, int x, int y) {
  return d->grid[d->Ymax * x + y].hardness;
}

int get_type(dungeon *d, int x, int y) { return d->grid[d->Ymax * x + y].type; }

void print_rooms_data(dungeon *d) {
  int i;
  for (i = 0; i < d->num_rooms; i++) {
    printf("\n--------------------------\n");
    printf("Data for room ID: %d\n", d->rooms[i].room_id);
    printf("X: %d, Y: %d\n", d->rooms[i].corner[0].x, d->rooms[i].corner[0].y);
    printf("Xlen: %d, Ylen: %d\n", d->rooms[i].Xlen, d->rooms[i].Ylen);
    printf("Area: %d\n", d->rooms[i].area);
  }
}

void destroy_dungeon(dungeon *d) {
  free(d->rooms);
  int i, j;
  for (i = 0; i < d->num_monsters; i++)
    destroy_player(&(d->monsters[i]));
  for(i = 0; i < d->Xmax; i++){
    for(j = 0; j < d->Ymax; j++){
      obj_stack_empty(&(d->grid[d->Ymax * i + j].objs));
    }
  }
  free(d->monsters);
  free(d->grid);
}
