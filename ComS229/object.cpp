#include "object.h"
#include "object_c.h"
#include <stdlib.h>
object::object(void){ this->has_object_field = 0; }

void object::set_name(string input){
  if(have_object_field(name))
    throw -1;
  this->name = input;
  set_object_field(name);
}

void object::set_symb(string input){
  if(have_object_field(symb))
    throw -1;
  this->symb = input;
  set_object_field(symb);
}

void object::set_desc(string input){
  if(have_object_field(desc))
    throw -1;
  this->desc = input;
  set_object_field(desc);
}

void object::set_type(string input){
  if(have_object_field(type_bit))
    throw -1;
  this->type = object_type(input);
  this->symb = (this->type).get_symbol();
  set_object_field(type_bit);
}

void object::set_color(string input){
  if(have_object_field(color))
    throw -1;
  this->color_type = color(input);
  set_object_field(color);
}

void object::set_hit(string input){
  if(have_object_field(hit))
    throw -1;
  int base, dice, sides;
  sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
  hit.set_base(base);
  hit.set_dice(dice);
  hit.set_sides(sides);
  set_object_field(hit);
}

void object::set_dam(string input){
  if(have_object_field(dam))
    throw -1;
  int base, dice, sides;
  sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
  dam.set_base(base);
  dam.set_dice(dice);
  dam.set_sides(sides);
  set_object_field(dam);
}

void object::set_dodge(string input){
  if(have_object_field(dodge))
    throw -1;
  int base, dice, sides;
  sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
  dodge.set_base(base);
  dodge.set_dice(dice);
  dodge.set_sides(sides);
  set_object_field(dodge);
}

void object::set_def(string input){
  if(have_object_field(def))
    throw -1;
  int base, dice, sides;
  sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
  def.set_base(base);
  def.set_dice(dice);
  def.set_sides(sides);
  set_object_field(def);
}

void object::set_weight(string input){
  if(have_object_field(weight))
    throw -1;
  int base, dice, sides;
  sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
  weight.set_base(base);
  weight.set_dice(dice);
  weight.set_sides(sides);
  set_object_field(weight);
}

void object::set_speed(string input){
  if(have_object_field(speed))
    throw -1;
  int base, dice, sides;
  sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
  speed.set_base(base);
  speed.set_dice(dice);
  speed.set_sides(sides);
  set_object_field(speed);
}

void object::set_attr(string input){
  if(have_object_field(attr))
    throw -1;
  int base, dice, sides;
  sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
  attr.set_base(base);
  attr.set_dice(dice);
  attr.set_sides(sides);
  set_object_field(attr);
}

void object::set_val(string input){
  if(have_object_field(val))
    throw -1;
  int base, dice, sides;
  sscanf(input.c_str(), "%d+%dd%d", &base, &dice, &sides);
  val.set_base(base);
  val.set_dice(dice);
  val.set_sides(sides);
  set_object_field(val);
}

int object::is_full(void){
  if(object_full)
    return 1;
  else
    return 0;
}

void object::print(void){
  cout << name << endl;
  cout << symb << endl;
  cout << desc;
  cout << type << endl;
  cout << color_type << endl;
  cout << hit << endl;
  cout << dam << endl;
  cout << dodge << endl;
  cout << def << endl;
  cout << weight << endl;
  cout << speed << endl;
  cout << attr << endl;
  cout << val << endl;
  
}

int generate_objects(object_t **objects, void * descriptions, int num_objects){
  std::vector<object> vec = ( *(std::vector<object> *) descriptions );
  int i;
  object type;

  // If not at least one object_template, cannot generate objects
  if(vec.size() < 1)
    return -1;

  // Allocate memory for the monsters
  *objects = (object_t *) malloc( sizeof(object_t) * num_objects );
  object_t *temp_obj = *objects;
  
  //  for(it = vec.begin(); it != vec.end(); it++){
  for(i = 0; i < num_objects; i++){
    // pick random monster template
    type = vec[rand() % vec.size()];
    c_dice damage;
    damage.base = type.dam.get_base();
    damage.dice = type.dam.get_dice();
    damage.sides = type.dam.get_sides();
    gen_obj(&(temp_obj[i]), type.type.index, type.name.c_str(), type.symb.c_str()[0], type.type.type.c_str(), type.color_type.number, type.desc.c_str(), type.speed.roll(), damage, type.hit.roll(), type.dodge.roll(), type.def.roll(), type.weight.roll(), type.attr.roll(), type.val.roll(), type.type.num_allowed, type.type.behavior);
  }
  return i;
}
