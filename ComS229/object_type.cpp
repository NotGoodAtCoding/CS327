#include "object_type.h"

ostream &operator<<(ostream &o, const object_type &input){
    return o << input.type;
}

string object_type::get_symbol(void){
  return this->symbol;
}