#include "monster.h"
#include <ncurses.h>
#include <stdlib.h>


monster::monster(void) { this->has_monster_field = 0; }

monster::monster(string name, char symb, string color, string desc) {
  this->name = name;
  this->symb = symb;
  this->color_type = color;
  this->desc = desc;
  this->has_monster_field = 0;
}

void monster::empty(void) { this->has_monster_field = 0; }

void monster::set_speed(string input) {
    if(have_monster_field(speed))
      throw "Trying to set mon_speed again\n";
    int base, dice, sides;
    sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
    speed.set_base(base);
    speed.set_dice(dice);
    speed.set_sides(sides);
    set_monster_field(speed);
}

void monster::set_damage(string input) {
    if(have_monster_field(damage))
      throw "Trying to set mon_damage again\n";
    int base, dice, sides;
    sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
    damage.set_base(base);
    damage.set_dice(dice);
    damage.set_sides(sides);
    set_monster_field(damage);
}

void monster::set_health(string input) {
    if(have_monster_field(health))
      throw "Trying to set mon_health again\n";
    int base, dice, sides;
    sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
    health.set_base(base);
    health.set_dice(dice);
    health.set_sides(sides);
    set_monster_field(health);
}

void monster::add_ability(string input) {
  ability to_add(input);
  vector<ability>::iterator it;
  int i = 0;
  for (it = abilities.begin(); it != abilities.end(); it++) {
    if(*it == to_add){i = 1; break;}
  }
  if(!i)
    abilities.push_back(to_add);
  else
    throw "Trying to set duplicate mon_ability\n";
}

void monster::set_abilities(stringstream &input){
    if(have_monster_field(abl))
      throw "Trying to set mon_abilities again\n";
    while(1){
      string part;
      input >> part;
      if(part != "")
        this->add_ability(part);
      if(input.tellg() == -1)
	break;
    }
    set_monster_field(abl);
}

void monster::set_name(string name) {
    if(have_monster_field(name))
      throw "Trying to set mon_name again\n";
    this->name = name;
    set_monster_field(name);
}

void monster::set_color(string input) {
    if(have_monster_field(color))
      throw "Trying to set mon_color again\n";
    color temp(input);
    this->color_type = temp;
    set_monster_field(color);
}

void monster::set_symb(string symb) {
    if(have_monster_field(symb))
      throw "Trying to set mon_symbol again\n";
    if(symb.size() != 1)
      throw "Trying to set mon_symbol not a char\n";
    this->symb = symb;
    set_monster_field(symb);
}

void monster::set_desc(string desc) {
    if(have_monster_field(desc))
      throw "Trying to set mon_desc again\n";
    this->desc = desc;
    set_monster_field(desc);
}

int monster::is_full() {
  if (monster_full)
    return 1;
  else
    return 0;
}

ostream &operator<< (ostream &o, const monster &monster){
  /* string temp;
  temp += monster.name;
  temp += "\n";*/
  o << monster.name << endl;
  o << monster.desc;
  o << monster.symb << endl;
  o << monster.color_type << endl;
  o << monster.speed << endl;
  vector<ability>::iterator it;
  vector<ability> vec = monster.abilities;
  int i = 0;
  for (it = vec.begin(); it != vec.end(); it++) {
    o << *it;
    i = 1;
    if(it+1 != vec.end())
      o << " ";
  }
  if(i)
    o << endl;
  
  o << monster.health << endl;
  o << monster.damage << endl;
  return o;
}

void monster::print(int x, int y) {

  attron(COLOR_PAIR(this->color_type.number));
  mvprintw(y, x, "Monster %s's symbols is %c", this->name.c_str(), this->symb.c_str()[0]);
  attroff(COLOR_PAIR(this->color_type.number));
  //refresh();
}


int generate_monsters(player_t **npc, void * descriptions, int num_monsters){
  std::vector<monster> vec = ( *(std::vector<monster> *) descriptions );
  std::vector<ability> abil_vec;
  std::vector<ability>::iterator abil;
  int i;
  monster type;

  // If not at least one monster_template, cannot generate monsters
  if(vec.size() < 1)
    return -1;

  // Allocate memory for the monsters
  *npc = (player_t *) malloc( sizeof(player_t) * num_monsters );
  player_t *temp_player = *npc;
  
  //  for(it = vec.begin(); it != vec.end(); it++){
  for(i = 0; i < num_monsters; i++){
    // pick random monster template
    type = vec[rand() % vec.size()];
    abil_vec = type.abilities;
    c_dice damage;
    damage.base = type.damage.get_base();
    damage.dice = type.damage.get_dice();
    damage.sides = type.damage.get_sides();
    gen_npc(&(temp_player[i]), type.name.c_str(), type.symb.c_str()[0], type.color_type.number, type.desc.c_str(), type.speed.roll(), damage, type.health.roll());

    // Iterate through and assign abilities.
    for(abil = abil_vec.begin(); abil != abil_vec.end(); abil++){
      if((*abil).get_name() == TELE)
        set_att(&(temp_player[i]), TELE);
      else if((*abil).get_name() == PASS)
        set_att(&(temp_player[i]), PASS);
      else if((*abil).get_name() == SMART)
        set_att(&(temp_player[i]), SMART);
      else if((*abil).get_name() == TUNNEL)
        set_att(&(temp_player[i]), TUNNEL);
    }
  }
  return i;
}
