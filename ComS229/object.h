#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <stdio.h>

#include "color.h"
#include "dice_param.h"
#include "object_type.h"

using namespace std;

#define object_name 0x00000001
#define object_desc 0x00000002
#define object_type_bit 0x00000004
#define object_color 0x00000008
#define object_hit 0x00000010
#define object_dam 0x00000020
#define object_dodge 0x00000040
#define object_def 0x00000080
#define object_weight 0x00000100
#define object_speed 0x00000200
#define object_attr 0x00000400
#define object_val 0x00000800
#define object_symb 0x00001000
#define object_BIT13 0x00002000
#define object_BIT14 0x00004000
#define object_BIT15 0x00008000
#define object_BIT16 0x00010000
#define object_BIT17 0x00020000
#define object_BIT18 0x00040000
#define object_BIT19 0x00080000
#define object_BIT20 0x00100000
#define object_BIT21 0x00200000
#define object_BIT22 0x00400000
#define object_BIT23 0x00800000
#define object_BIT24 0x01000000
#define object_BIT25 0x02000000
#define object_BIT26 0x04000000
#define object_BIT27 0x08000000
#define object_BIT28 0x10000000
#define object_BIT29 0x20000000
#define object_BIT30 0x40000000
#define object_BIT31 0x80000000

#define num_object_VALID 13

#define object_full (((1u << num_object_VALID) - 1) == this->has_object_field)

#define have_object_field(bit) this->has_object_field &object_##bit

#define set_object_field(bit) this->has_object_field |= object_##bit

class object {


public:
  string name;
  string desc;
  string symb;
  object_type type;
  color color_type;
  dice_param hit;
  dice_param dam;
  dice_param dodge;
  dice_param def;
  dice_param weight;
  dice_param speed;
  dice_param attr;
  dice_param val;
  
  uint32_t has_object_field;

  object(void);

  void set_name(string input);

  void set_desc(string input);

  void set_type(string input);
  
  void set_symb(string input);

  void set_color(string input);
  
  void set_hit(string input);
  
  void set_dam(string input);
  
  void set_dodge(string input);
  
  void set_def(string input);
  
  void set_weight(string input);
  
  void set_speed(string input);
  
  void set_attr(string input);
  
  void set_val(string input);
  
  int is_full(void);
  
  void print(void);
};