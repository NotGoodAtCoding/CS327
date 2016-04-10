#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <stdio.h>

#include "color.h"
#include "ability.h"
#include "dice_param.h"

#include "player.h"
#include "c_dice.h"
#include "monster_c.h"

using namespace std;

#define monster_name 0x00000001
#define monster_color 0x00000002
#define monster_symb 0x00000004
#define monster_desc 0x00000008
#define monster_speed 0x00000010
#define monster_damage 0x00000020
#define monster_health 0x00000040
#define monster_abl 0x00000080
#define monster_BIT08 0x00000100
#define monster_BIT09 0x00000200
#define monster_BIT10 0x00000400
#define monster_BIT11 0x00000800
#define monster_BIT12 0x00001000
#define monster_BIT13 0x00002000
#define monster_BIT14 0x00004000
#define monster_BIT15 0x00008000
#define monster_BIT16 0x00010000
#define monster_BIT17 0x00020000
#define monster_BIT18 0x00040000
#define monster_BIT19 0x00080000
#define monster_BIT20 0x00100000
#define monster_BIT21 0x00200000
#define monster_BIT22 0x00400000
#define monster_BIT23 0x00800000
#define monster_BIT24 0x01000000
#define monster_BIT25 0x02000000
#define monster_BIT26 0x04000000
#define monster_BIT27 0x08000000
#define monster_BIT28 0x10000000
#define monster_BIT29 0x20000000
#define monster_BIT30 0x40000000
#define monster_BIT31 0x80000000

#define num_monster_VALID 8

#define monster_full (((1u << num_monster_VALID) - 1) == this->has_monster_field)

#define have_monster_field(bit) this->has_monster_field &monster_##bit

#define set_monster_field(bit) this->has_monster_field |= monster_##bit

class monster {



public:

  string name;
  string symb;
  color color_type;
  string desc;
  dice_param speed;
  dice_param damage;
  dice_param health;
  vector<ability> abilities;
  uint32_t has_monster_field;
  
  
  monster(void);

  monster(string name, char symb, string color, string desc);

  void empty(void);

  void set_speed(string input);

  void set_damage(string input);

  void set_health(string input);

  void set_abilities(stringstream &input);

  void add_ability(string input);

  void set_name(string name);

  void set_color(string input);

  void set_symb(string symb);

  void set_desc(string desc);

  int is_full();

  void print(int x, int y);

  friend ostream &operator<< (ostream &o, const monster &monster);
  
};
