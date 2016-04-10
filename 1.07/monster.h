#include <string>
#include <vector>

#include "dice.h"
# ifdef __cplusplus
extern "C" {
# endif

using namespace std;
class monster {

public:
    string name;
    string desc;
    string symb;
    string color;
    dice hp;
    dice speed;
    dice dam;
    vector<string> abilities;

    monster(void);
    monster(char *name, char *desc, char symb);

    void set_hp(dice in);
    void set_speed(dice in);
    void set_dam(dice in);
    void add_ability(string ability);
    void set_abilities(vector<string> ability);
    void set_desc(string desc);
    void set_color(string color);
};

# ifdef __cplusplus
}
#endif
