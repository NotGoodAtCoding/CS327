//
// Created by Stefan Kraus on 3/28/2016.
//

#include <fstream>
#include <iostream>
#include <vector>

#include "parser.h"
#include "monster.h"
#include "object.h"


using namespace std;

string truncate(string line){
    return line.substr(line.find(" ")+1, line.length());
}

void print_die(string title, dice d){
    cout <<title+" " << d.base << "+" << d.num << "d" << d.sides <<endl;
}

void print_vector(vector<string> v){
    for(uint16_t i=0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}

int print_monsters(std::vector<monster> &monsters);

dice parse_dice(string code){
    int base, num, sides;
    //scans the string into variables using a format string
    sscanf(code.c_str(), "%d+%dd%d", &base, &num, &sides);
    dice temp = dice(base, num, sides);
    cout << temp.base << " " << temp.num << endl;
    return dice(base, num, sides);
}

void parse_monsters(char * file)
{
    vector<monster> monsters;

    //Filestream
    string input_file(file);
    ifstream in(input_file.c_str());

    //Current Line
    string line;

    //State of the parser
    state parse_state = STANDBY;

    //Get the first line in the file
    getline(in, line);
    //Make sure it has correct header. find() returns npos() if not found
    if(line.find(monster_file_header) != string::npos){

        //Get next line
        while(getline(in, line) && parse_state != ERR){
            switch (parse_state) {
                case ERR:
                    break;
                case STANDBY :
                    if(line.find(monster_start) != string::npos){
                        parse_state = READING;
                        monsters.push_back(monster());
                    }
                    break;
                case READING :
                    if(line.find(monster_end) != string::npos){
                        parse_state = STANDBY;
                        break;
                    } else if(line.find("NAME") != string::npos){
                        monsters.back().name = truncate(line);
                        break;
                    } else if(line.find("SYMB") != string::npos){
                        monsters.back().symb = truncate(line);
                        break;
                    }else if(line.find("HP")  != string::npos ) {
                        monsters.back().set_hp(parse_dice(truncate(line)));
                        break;
                    } else if(line.find("SPEED")  != string::npos ) {
                        monsters.back().set_speed(parse_dice(truncate(line)));
                        break;
                    } else if(line.find("DAM")  != string::npos ) {
                        monsters.back().set_dam(parse_dice(truncate(line)));
                        break;
                    } else if(line.find("DESC") != string::npos){
                        parse_state = DESCRIPTION;
                        break;
                    } else if(line.find("COLOR") != string::npos){
                        monsters.back().set_color(truncate(line));
                        break;
                    } else if(line.find("ABIL") != string::npos){
                        while(line.find(" ") != string::npos) {
                            line = truncate(line);
                            monsters.back().add_ability(line.substr(0, line.find(" ")));
                        }
                        break;
                    }
                    break;
                case DESCRIPTION :
                    string desc = "";
                    while(getline(in, line) && parse_state == DESCRIPTION){
                        if(line != "."){
                            desc += line + "\n";
                        } else {
                            monsters.back().set_desc(desc);
                            parse_state = READING;
                        }
                    }
                    break;

            }
        }
    }
    print_monsters(monsters);
}

int print_monsters(std::vector<monster> &monsters) {
    for(uint16_t i=0; i < monsters.size(); i++){
        cout << monsters[i].name << endl;
        cout << monsters[i].symb << endl;
        cout << monsters[i].color << endl;
        cout << monsters[i].desc << endl;
        print_die("SPEED", monsters[i].speed);
        print_vector(monsters[i].abilities);
        print_die("DAM", monsters[i].dam);
        print_die("HP", monsters[i].hp);
        cout << endl;
    }
    return 0;
}

void print_objects(vector<object> objs){
    for(uint16_t i=0; i < objs.size(); i++){
        cout << objs[i].name << endl;
        cout << objs[i].description << endl;
        cout << objs[i].type << endl;
        cout << objs[i].color << endl;
        print_die("hit_bonus", objs[i].hit_bonus);
        print_die("dam_bonus", objs[i].dam_bonus);
        print_die("dge_bonus", objs[i].dge_bonus);
        print_die("def_bonus", objs[i].def_bonus);
        print_die("weight", objs[i].weight);
        print_die("spd_bonus", objs[i].spd_bonus);
        print_die("value", objs[i].value);
        cout << endl;
    }
}

void parse_objects(char * file)
{

    vector<object> objects;

    //Filestream
    string input_file(file);
    ifstream in(input_file.c_str());

    //Current Line
    string line;

    //State of the parser
    state parse_state = STANDBY;

    //Get the first line in the file
    getline(in, line);
    //Make sure it has correct header. find() returns npos() if not found
    if(line.find(object_file_header) != string::npos){

        //Get next line
        while(getline(in, line) && parse_state != ERR){
            switch (parse_state) {
                case ERR:
                    break;
                case STANDBY :
                    if(line.find(object_start) != string::npos){
                        parse_state = READING;
                        objects.push_back(object());
                    }
                    break;
                case READING :
                    if(line.find(object_end) != string::npos){
                        parse_state = STANDBY;
                        break;
                    } else if(line.find("NAME") != string::npos){
                        objects.back().name = truncate(line);
                        break;
                    } else if(line.find("HIT")  != string::npos ) {
                        objects.back().set_hit_bonus(parse_dice(truncate(line)));
                        break;
                    } else if(line.find("DAM")  != string::npos ) {
                        objects.back().set_dam_bonus(parse_dice(truncate(line)));
                        break;
                    } else if(line.find("DODGE")  != string::npos ) {
                        objects.back().set_dge_bonus(parse_dice(truncate(line)));
                        break;
                    } else if(line.find("DEF")  != string::npos ) {
                        objects.back().set_def_bonus(parse_dice(truncate(line)));
                        break;
                    } else if(line.find("WEIGHT")  != string::npos ) {
                        objects.back().set_weight(parse_dice(truncate(line)));
                        break;
                    } else if(line.find("SPEED")  != string::npos ) {
                        objects.back().set_spd_bonus(parse_dice(truncate(line)));
                        break;
                    }else if(line.find("ATTR")  != string::npos ) {
                        objects.back().set_spc_value(parse_dice(truncate(line)));
                        break;
                    } else if(line.find("DESC") != string::npos){
                        parse_state = DESCRIPTION;
                        break;
                    } else if(line.find("COLOR") != string::npos){
                        objects.back().set_color(truncate(line));
                        break;
                    } else if(line.find("TYPE") != string::npos){
                        objects.back().set_type(truncate(line));

                        break;
                    }
                    break;
                case DESCRIPTION :
                    string desc = "";
                    while(getline(in, line) && parse_state == DESCRIPTION){
                        if(line != "."){
                            desc += line + "\n";
                        } else {
                            objects.back().set_desc(desc);
                            parse_state = READING;
                        }
                    }
                    break;

            }
        }
    }
    print_objects(objects);
}