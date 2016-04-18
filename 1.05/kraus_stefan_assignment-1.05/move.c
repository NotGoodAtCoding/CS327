#include "move.h"

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

#include "dungeon.h"
#include "heap.h"
#include "move.h"
#include "npc.h"
#include "pc.h"
#include "character.h"
#include "utils.h"
#include "path.h"

void do_combat(dungeon_t *d, character_t *atk, character_t *def)
{
  def->alive = 0;
  if (def != &d->pc) {
    d->num_monsters--;
  }
}

void move_character(dungeon_t *d, character_t *c, pair_t next)
{
  if (charpair(next) &&
      ((next[dim_y] != c->position[dim_y]) ||
       (next[dim_x] != c->position[dim_x]))) {
    do_combat(d, c, charpair(next));
  } else {
    /* No character in new position. */

    d->character[c->position[dim_y]][c->position[dim_x]] = NULL;
    c->position[dim_y] = next[dim_y];
    c->position[dim_x] = next[dim_x];
    d->character[c->position[dim_y]][c->position[dim_x]] = c;
  }
}

//---ADDED FUNCTIONS ----
void user_action(dungeon_t *d, character_t *c) {
  pair_t next;
  //uint32_t moveDecision;
  char ch = getchar();

  switch(ch) {
  case '7' :
  case 'y' :
    next[dim_x] = c->position[dim_x] - 1;
    next[dim_y] = c->position[dim_y] - 1;
    break;
  case '8' :
  case 'k' :
    next[dim_x] = c->position[dim_x];
    next[dim_y] = c->position[dim_y] - 1;
    break;
  case '9' :
  case 'u' :
    next[dim_x] = c->position[dim_x] + 1;
    next[dim_y] = c->position[dim_y] - 1;
    break;
  case '6' :
  case 'l' :
    next[dim_x] = c->position[dim_x] + 1;
    next[dim_y] = c->position[dim_y];
    break;
  case '3' :
  case 'n' :
    next[dim_x] = c->position[dim_x] + 1;
    next[dim_y] = c->position[dim_y] + 1;
    break;
  case '2' :
  case 'j' :
    next[dim_x] = c->position[dim_x];
    next[dim_y] = c->position[dim_y] + 1;
    break;
  case '1' :
  case 'b' :
    next[dim_x] = c->position[dim_x] - 1;
    next[dim_y] = c->position[dim_y] + 1;
    break;
  case '4' :
  case 'h' :
    next[dim_x] = c->position[dim_x] - 1;
    next[dim_y] = c->position[dim_y];
    break;

    /* Quit Game by killing Player */
  case 's': 
    c->alive = 0;
    return;
    break;

  case ' ':
  default:
    next[dim_x] = c->position[dim_x];
    next[dim_y] = c->position[dim_y];
    break;
  }

  if(d->map[next[dim_y]][next[dim_x]] == ter_wall ||
     d->map[next[dim_y]][next[dim_x]] == ter_wall_immutable){
    return;
  }

  move_character(d, c, next);
}
//----END ADDED FUNCTIONS

void do_moves(dungeon_t *d)
{
  pair_t next;
  character_t *c;

  /* Remove the PC when it is PC turn.  Replace on next call.  This allows  *
   * user to completely uninit the heap when generating a new level without *
   * worrying about deleting the PC.                                        */

  if (pc_is_alive(d)) {
    heap_insert(&d->next_turn, &d->pc);
  }

  while (pc_is_alive(d) && ((c = heap_remove_min(&d->next_turn)) != &d->pc)) {
    if (!c->alive) {
      if (d->character[c->position[dim_y]][c->position[dim_x]] == c) {
        d->character[c->position[dim_y]][c->position[dim_x]] = NULL;
      }
      if (c != &d->pc) {
        character_delete(c);
      }
      continue;
    }

    c->next_turn += (1000 / c->speed);

    npc_next_pos(d, c, next);
    move_character(d, c, next);

    heap_insert(&d->next_turn, c);
  }

  if (pc_is_alive(d) && c == &d->pc) {
    user_action(d, c);
    pc_next_pos(d, next);
    next[dim_x] += c->position[dim_x];
    next[dim_y] += c->position[dim_y];
    if (mappair(next) <= ter_floor) {
      mappair(next) = ter_floor_hall;
    }

    dijkstra(d);
    dijkstra_tunnel(d);

    c->next_turn += (1000 / c->speed);
  }
}

void dir_nearest_wall(dungeon_t *d, character_t *c, pair_t dir)
{
  dir[dim_x] = dir[dim_y] = 0;

  if (c->position[dim_x] != 1 && c->position[dim_x] != DUNGEON_X - 2) {
    dir[dim_x] = (c->position[dim_x] > DUNGEON_X - c->position[dim_x] ? 1 : -1);
  }
  if (c->position[dim_y] != 1 && c->position[dim_y] != DUNGEON_Y - 2) {
    dir[dim_y] = (c->position[dim_y] > DUNGEON_Y - c->position[dim_y] ? 1 : -1);
  }
}

uint32_t in_corner(dungeon_t *d, character_t *c)
{
  uint32_t num_immutable;

  num_immutable = 0;

  num_immutable += (mapxy(c->position[dim_x] - 1,
                          c->position[dim_y]    ) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x] + 1,
                          c->position[dim_y]    ) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x]    ,
                          c->position[dim_y] - 1) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x]    ,
                          c->position[dim_y] + 1) == ter_wall_immutable);

  return num_immutable > 1;
}

uint32_t move_pc(dungeon_t *d, uint32_t dir)
{
  return 1;
}
