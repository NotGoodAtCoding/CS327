#include "color.h"


ostream &operator<<(ostream &o, const color &input){
    return o << input.type;
}
