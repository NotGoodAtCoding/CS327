#include "ability.h"

ostream &operator<<(ostream &o, const ability &input){
    return o << input.name;
}

const char *ability::get_c_name(void){
  return this->name.c_str();
}

string ability::get_name(void){
  return this->name;
}
