#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

#define SMART "SMART"
#define TELE "TELE"
#define PASS "PASS"
#define TUNNEL "TUNNEL"

class ability {
private:
  string name;

public:
  ability(string input) {
    if (input == SMART)
      name = SMART;
    else if (input == TELE)
      name = TELE;
    else if (input == PASS)
      name = PASS;
    else if (input == TUNNEL)
      name = TUNNEL;
    else {
      name = "<VOID>";
      char msg[80];
      sprintf(msg, "Ability mon_%s not supported\n", input.c_str());
      throw msg;
    }
  }
  
  inline int operator==(const ability &a){
    return this->name == a.name;
  }

  const char *get_c_name(void);
  
  string get_name(void);

  friend ostream &operator<<(ostream &o, const ability &input);
};
