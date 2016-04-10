#include <iostream>

#include "object_c.h"

using namespace std;

#define WEAPON "WEAPON"
#define OFFHAND "OFFHAND"
#define RANGED "RANGED"
#define ARMOR "ARMOR"
#define HELMET "HELMET"
#define CLOAK "CLOAK"
#define GLOVES "GLOVES"
#define BOOTS "BOOTS"
#define RING "RING"
#define AMULET "AMULET"
#define LIGHT "LIGHT"
#define SCROLL "SCROLL"
#define BOOK "BOOK"
#define FLASK "FLASK"
#define GOLD "GOLD"
#define AMMUNITION "AMMUNITION"
#define FOOD "FOOD"
#define WAND "WAND"
#define CONTAINER "CONTAINER"

class object_type{

public:
  string type;
  string symbol;
  int num_allowed;
  int behavior;
  object_e index;
  object_type(void){}
  
  string get_symbol(void);
  
  object_type(string input){
    if(input == WEAPON){
      type = WEAPON;
      symbol = "|";
      num_allowed = 1;
      behavior = 1;
      index = WEAPON_E;
    }
    else if(input == OFFHAND){
      type = OFFHAND;
      symbol = ")";
      num_allowed = 1;
      behavior = 1;
      index = OFFHAND_E;
    }
    else if(input == RANGED){
      type = RANGED;
      symbol = "}";
      num_allowed = 1;
      behavior = 1;
      index = RANGED_E;
    }
    else if(input == ARMOR){
      type = ARMOR;
      symbol = "[";
      num_allowed = 1;
      behavior = 1;
      index = ARMOR_E;
    }
    else if(input == HELMET){
      type = HELMET;
      symbol = "]";
      num_allowed = 1;
      behavior = 1;
      index = HELMET_E;
    }
    else if(input == CLOAK){
      type = CLOAK;
      symbol = "(";
      num_allowed = 1;
      behavior = 1;
      index = CLOAK_E;
    }
    else if(input == GLOVES){
      type = GLOVES;
      symbol = "{";
      num_allowed = 1;
      behavior = 1;
      index = GLOVES_E;
    }
    else if(input == BOOTS){
      type = BOOTS;
      symbol = "\\";
      num_allowed = 1;
      behavior = 1;
      index = BOOTS_E;
    }
    else if(input == RING){
      type = RING;
      symbol = "=";
      num_allowed = 2;
      behavior = 1;
      index = RING_E;
    }
    else if(input == AMULET){
      type = AMULET;
      symbol = "\"";
      num_allowed = 1;
      behavior = 1;
      index = AMULET_E;
    }
    else if(input == LIGHT){
      type = LIGHT;
      symbol = "_";
      num_allowed = 1;
      behavior = 1;
      index = LIGHT_E;
    }
    else if(input == SCROLL){
      type = SCROLL;
      symbol = "~";
      num_allowed = 0;
      behavior = 1;
      index = SCROLL_E;
    }
    else if(input == BOOK){
      type = BOOK;
      symbol = "?";
      num_allowed = 0;
      behavior = 1;
      index = BOOK_E;
    }
    else if(input == FLASK){
      type = FLASK;
      symbol = "!";
      num_allowed = 0;
      behavior = 1;
      index = FLASK_E;
    }
    else if(input == GOLD){
      type = GOLD;
      symbol = "$";
      num_allowed = 0;
      behavior = 2;
      index = GOLD_E;
    }
    else if(input == AMMUNITION){
      type = AMMUNITION;
      symbol = "/";
      num_allowed = 0;
      behavior = 2;
      index = AMMUNITION_E;
    }
    else if(input == FOOD){
      type = FOOD;
      symbol = ",";
      num_allowed = 0;
      behavior = 2;
      index = FOOD_E;
    }
    else if(input == WAND){
      type = WAND;
      symbol = "-";
      num_allowed = 0;
      behavior = 1;
      index = WAND_E;
    }
    else if(input == CONTAINER){
      type = CONTAINER;
      symbol = "%";
      behavior = 3;
      num_allowed = 0;
      index = CONTAINER_E;
    }
    else{
      type = "<VOID>";
      throw -1;
    }
  }
  
   friend ostream &operator<<(ostream &o, const object_type &input);

};