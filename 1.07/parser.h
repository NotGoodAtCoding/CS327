//
// Created by Stefan Kraus on 3/28/2016.
//

#include "dice.h"

#define monster_file_header "RLG327 MONSTER DESCRIPTION 1"
#define monster_start "BEGIN MONSTER"
#define monster_end "END"

#define object_file_header "RLG327 OBJECT DESCRIPTION 1"
#define object_start "BEGIN OBJECT"
#define object_end "END"

enum state {
    STANDBY,    //Not reading a monster
    READING,    //Reading a monster
    DESCRIPTION,//reading a description
    ERR       //Encountered an error
};

# ifdef __cplusplus
extern "C" {
# endif

//dice parse_dice(char *code);
void parse_monsters(char *file);
void parse_objects(char *file);

# ifdef __cplusplus
}
#endif

