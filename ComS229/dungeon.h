#ifndef __DUNGEON
#define __DUNGEON

#include "player.h"
#include "point.h"
#include "room.h"
#include "monster_c.h"
#include "object_c.h"

#include "stack.h"

#define WIN_LEN 80
#define WIN_HEIGHT 24

typedef enum floor_type{
  ROOM,
  PATH,
  WALL,
  STONE,
  STAIRS_UP,
  STAIRS_DOWN,
  DEBUG
}floor_type;

typedef struct tile{
  unsigned char hardness;
  floor_type type;
  player_t *ocupant;
  int label;
  obj_stack_t objs;
}tile;

typedef struct dungeon{
  int Xmax;
  int Ymax;
  int Xwin;
  int Ywin;
  int win_len;
  int win_height;
  int area;
  int16_t num_rooms;
  int16_t num_monsters;
  int16_t num_monsters_alive;
  int version;
  tile *grid;
  room *rooms;
  player_t *monsters;
  player_t *pc;
  object_t *objects;
  int16_t num_objects;
}dungeon;

void debug_next_move(dungeon *d);

void print_dungeon(dungeon *d);

void save_dungeon(dungeon *d);

int load_dungeon(dungeon *d, player_t *pc);

int generate_dungeon(dungeon *d, player_t *pc, int num_rooms, int num_monsters, void * monster_templates, int num_objects, void * object_templates, int Xmax, int Ymax);

int populate_monsters(dungeon *d);

int place_monsters(dungeon *d);

int place_objects(dungeon *d);

int place_object(dungeon *d, int x, int y, object_t *obj);

int place_character(dungeon *d, player_t *player);

void center_on_pc(dungeon *d);

void look_around(dungeon *d);

player_t *is_ocupied(dungeon *d, point_type location);

int num_objs(dungeon *d, int x, int y);

int pop_objs(dungeon *d, int x, int y, object_t *objs);

object_t * peek_objs(dungeon *d, int x, int y);

void place_ocupant(dungeon *d, int x, int y, player_t *player);

int last_seen(dungeon *d, player_t *p);

int same_room(dungeon *d, point_type A, point_type B);

int meander(dungeon *d, point_type start, point_type *next);

int populate_rooms(dungeon *d);

void print_rooms_data(dungeon *d);

void finalize_rooms(dungeon *d);

int place_rooms(dungeon *d);

int safe_location(dungeon *d, int x, int y, int room_id);

void label_room(dungeon *d, int id, int x, int y);

void destroy_dungeon(dungeon *d);

int connect_rooms(dungeon *d);

void print_path(dungeon *d, point_type start, point_type end);

int move_ocupant(dungeon *d, player_t *p);

int dumb_next_move(dungeon *d, point_type start, point_type end, point_type *next);

int smart_next_move(dungeon *d, point_type start, point_type end, point_type *next);

void dij(dungeon *d, point_type start, point_type end, point_type *parent, int fast);

int carve_path(dungeon *d, int start_room, int end_room);

void place(dungeon *d, int x, int y, unsigned char thing);

void place_type(dungeon *d, int x, int y, floor_type type);

unsigned char get(dungeon *d, int x, int y);

int get_type(dungeon *d, int x, int y);
  
void neighbor (point_type source, point_type *neighbors);

#endif
