//PLAYER

#ifndef __player
#define __player

#include "stdint.h"

#include "point.h"
#include "c_dice.h"
#include "object_c.h"
#include "linked_list.h"

typedef struct NPC{
  uint32_t attr;
  char *description;
  char *name;
  point_type last_seen;
}NPC_t;

typedef struct PC{
  object_list_t equiped;
  object_list_t standby;
  int standby_max;
  int weapon;
  uint32_t act_speed;
}PC_t;

typedef union player_attr{
  NPC_t npc;
  PC_t pc;
}player_attr_u;

typedef struct player{
  player_attr_u attr;
  point_type position;
  int index;
  char symbol;
  signed int hitpoints;
  c_dice damage;
  int speed;
  int turn;
  short int color_num;
}player_t;

#define NPC_SMART         0x00000001
#define NPC_TELE          0x00000002
#define NPC_SEEN          0x00000004
#define NPC_DEAD          0x00000008
#define NPC_PASS          0x00000010
#define NPC_TUNNEL        0x00000020
#define NPC_BIT06         0x00000040
#define NPC_BIT07         0x00000080
#define NPC_BIT08         0x00000100
#define NPC_BIT09         0x00000200
#define NPC_BIT10         0x00000400
#define NPC_BIT11         0x00000800
#define NPC_BIT12         0x00001000
#define NPC_BIT13         0x00002000
#define NPC_BIT14         0x00004000
#define NPC_BIT15         0x00008000
#define NPC_BIT16         0x00010000
#define NPC_BIT17         0x00020000
#define NPC_BIT18         0x00040000
#define NPC_BIT19         0x00080000
#define NPC_BIT20         0x00100000
#define NPC_BIT21         0x00200000
#define NPC_BIT22         0x00400000
#define NPC_BIT23         0x00800000
#define NPC_BIT24         0x01000000
#define NPC_BIT25         0x02000000
#define NPC_BIT26         0x04000000
#define NPC_BIT27         0x08000000
#define NPC_BIT28         0x10000000
#define NPC_BIT29         0x20000000
#define NPC_BIT30         0x40000000
#define NPC_BIT31         0x80000000

#define NUM_VALID 6

# define has_att(player, bit)              	\
  ((player)->attr.npc.attr & NPC_##bit)
	  
# define set_att(player, bit)				\
  ((player)->attr.npc.attr |= NPC_##bit)  

# define reset_att(player, bit)\
  ((player)->attr.npc.attr &= ~NPC_##bit)
	
#ifdef __cplusplus
extern "C" {
#endif
	
void gen_pc(player_t *pc);
  
void set_position(player_t *p, point_type position);

int set_index(player_t *p, int index);

void set_next_index(int index);

int get_next_index(void);

void set_turn(player_t *p, int turn);

void reset_pc(player_t *pc);
	
int gen_npc(player_t *npc, const char *name, char symbol, short int color, const char *description, int speed, c_dice damage, int health);

void destroy_player(player_t *p);

void player_print(player_t *player, int x, int y);

int player_pickup(player_t *pc, object_t *obj);

int player_attack(player_t *pc, player_t *monster);

#ifdef __cplusplus
}
#endif

#endif
