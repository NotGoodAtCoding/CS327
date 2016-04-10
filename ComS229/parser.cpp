#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string.h>

#include "parser.h"
#include "monster.h"
#include "object.h"

#define DEBUG 1

using namespace std;

void advance(string &input) {}

int parse_monsters(std::vector<monster> &monsters, string file);

int parse_objects(std::vector<object> &objects, string file);

int print_monsters(std::vector<monster> &monsters);

int print_objects(std::vector<object> &objects);

int parse_monsters(std::vector<monster> &monsters, string file) {

  // stream to read from file with
  ifstream input(file.c_str());

  // current line of file being parsed
  string line;
  // if true, currently reading monster descriptin.
  int reading = 0;

  // get first line of file, check for descripter_key
  getline(input, line);
  if (line.find(monster_descripter_key) != string::npos) {

    // loop through rest of file
    while (!input.eof()) {

      // check for begin of monster
      if (line.find(monster_begin_key) != string::npos) {
        reading = 1;
        // add new monster to vector
        monsters.push_back(monster());
      }

      // check for end of monster
      if (line.find(end_key) != string::npos) {
        if (monsters.back().is_full() == 0 && reading != 0){
          if(DEBUG)
            printf("Not enough lines for monster\n");
          monsters.pop_back();
        }
        reading = 0;
      }

      // get next line to parse
      getline(input, line);

      // only if reading
      if (reading) {
        try {
          stringstream stream(line);
          string junk;
          string parsed;
          stream >> junk;

          // line contains NAME information
          if (line.find(NAME) != string::npos) {
            // get entire name
            while (stream.tellg() != -1) {
              string part;
              stream >> part;
              parsed += part;
              if (stream.tellg() != -1)
                parsed += " ";
            }
            monsters.back().set_name(parsed);

            // Line contains SYMBOL information
          } else if (line.find(SYMBOL) != string::npos) {
            stream >> parsed;
            monsters.back().set_symb(parsed);
            if (stream >> parsed)
              throw "Too many on mon_symbol line\n";

            // line contains COLOR information
          } else if (line.find(COLOR) != string::npos) {
            stream >> parsed;
            monsters.back().set_color(parsed);
            if (stream >> parsed)
              throw "Too many on mon_color line\n";

            // line contains DESCRIPTION information
          } else if (line.find(DESCRIPTION) != string::npos) {
            string desc;
            int flag = 0;
            while (!input.eof()) {
              getline(input, line);
              if (line != ".") {
                desc += line;
                desc += "\n";
                flag = 1;
              } else{
                if(!flag)
                  desc += "\n";
                break;
              }
            }
            monsters.back().set_desc(desc);

            // line contains ABILITIES information
          } else if (line.find(ABILITY) != string::npos) {
            monsters.back().set_abilities(stream);

            // line contains SPEED information
          } else if (line.find(SPEED) != string::npos) {
            stream >> parsed;
            monsters.back().set_speed(parsed);
            if (stream >> parsed)
              throw "Too many on mon_speed line\n";

            // line contains DAMAGE information
          } else if (line.find(DAMAGE) != string::npos) {
            stream >> parsed;
            monsters.back().set_damage(parsed);
            if (stream >> parsed)
              throw "Too many on mon_dam line\n";

            // line contains HEALTH information
          } else if (line.find(HEALTH) != string::npos) {
            stream >> parsed;
            monsters.back().set_health(parsed);
            if (stream >> parsed)
              throw "Too many on mon_health line\n";
          }
         // throw "test\n";
        } catch (...) {
          monsters.pop_back();
          reading = 0;
          if(DEBUG)
            //printf("Error: %s", s);
          continue;
        }
      }
    }
  }
   if (monsters.back().is_full() == 0  || reading == 1)
     monsters.pop_back();
  input.close();

  return 0;
}

int parse_objects(std::vector<object> &objects, string file) {

  // stream to read from file with
  ifstream input(file.c_str());

  // current line of file being parsed
  string line;
  // if true, currently reading object descriptin.
  int reading = 0;

  // get first line of file, check for descripter_key
  getline(input, line);
  if (line.find(object_descripter_key) != string::npos) {

    // loop through rest of file
    while (!input.eof()) {

      // check for begin of object
      if (line.find(object_begin_key) != string::npos) {
        reading = 1;
        // add new object to vector
        objects.push_back(object());
        objects.back().set_symb(".");
      }

      // check for end of object
      if (line.find(end_key) != string::npos) {
        if (objects.back().is_full() == 0 && reading != 0)
          objects.pop_back();
        reading = 0;
      }

      // get next line to parse
      getline(input, line);

      // only if reading
      if (reading) {
        try {

          stringstream stream(line);
          string junk;
          string parsed;
          stream >> junk;

          // line contains NAME information
          if (line.find(NAME) != string::npos) {
            // get entire name
            while (stream.tellg() != -1) {
              string part;
              stream >> part;
              parsed += part;
              if (stream.tellg() != -1)
                parsed += " ";
            }
            objects.back().set_name(parsed);

            // Line contains SYMBOL information
            /*} else if (line.find(SYMBOL) != string::npos) {
            stream >> parsed;
            objects.back().set_symb(parsed);
            if (stream >> parsed)
              throw - 1;
            */
            // line contains COLOR information
          } else if (line.find(COLOR) != string::npos) {
            stream >> parsed;
            objects.back().set_color(parsed);
            if (stream >> parsed)
              throw - 1;

            // line contains DESCRIPTION information
          } else if (line.find(DESCRIPTION) != string::npos) {
            int flag = 0;
            string desc;
            while (!input.eof()) {
              getline(input, line);
              if (line != ".") {
                desc += line;
                desc += "\n";
                flag = 1;
              } else{
                if(!flag)
                  desc += "\n";
                break;
              }
            }
            objects.back().set_desc(desc);

            // line contains SPEED information
          } else if (line.find(SPEED) != string::npos) {
            stream >> parsed;
            objects.back().set_speed(parsed);
            if (stream >> parsed)
              throw - 1;

            // line contains DAMAGE information
          } else if (line.find(DAMAGE) != string::npos) {
            stream >> parsed;
            objects.back().set_dam(parsed);
            if (stream >> parsed)
              throw - 1;

            // line contains TYPE information
          } else if (line.find(TYPE) != string::npos) {
            stream >> parsed;
            objects.back().set_type(parsed);
            if (stream >> parsed)
              throw - 1;

            // line contains HIT information
          } else if (line.find(HIT) != string::npos) {
            stream >> parsed;
            objects.back().set_hit(parsed);
            if (stream >> parsed)
              throw - 1;

            // line contains DODGE information
          } else if (line.find(DODGE) != string::npos) {
            stream >> parsed;
            objects.back().set_dodge(parsed);
            if (stream >> parsed)
              throw - 1;

            // line contains DEFENSIVE information
          } else if (line.find(DEFENSIVE) != string::npos) {
            stream >> parsed;
            objects.back().set_def(parsed);
            if (stream >> parsed)
              throw - 1;

            // line contains WEIGHT information
          } else if (line.find(WEIGHT) != string::npos) {
            stream >> parsed;
            objects.back().set_weight(parsed);
            if (stream >> parsed)
              throw - 1;

            // line contains ATTRIBUTE information
          } else if (line.find(ATTRIBUTE) != string::npos) {
            stream >> parsed;
            objects.back().set_attr(parsed);
            if (stream >> parsed)
              throw - 1;

            // line contains VALUE information
          } else if (line.find(VALUE) != string::npos) {
            stream >> parsed;
            objects.back().set_val(parsed);
            if (stream >> parsed)
              throw - 1;
          }
        } catch (...) {
          objects.pop_back();
          reading = 0;
          continue;
        }
      }
    }
  }
  if (objects.back().is_full() == 0  || reading == 1)
    objects.pop_back();
  input.close();

  return 0;
}

int print_monsters(std::vector<monster> &monsters) {
  vector<monster>::iterator it = monsters.begin();
  int y = 0;
  for (it = monsters.begin(); it != monsters.end(); ++it) {
    it->print(0, y++);
  
    //  cout << *it << endl;

        if (it + 1 != monsters.end())
      cout << endl;
    
  }
  return 0;
}

int print_objects(std::vector<object> &objects) {
  vector<object>::iterator it = objects.begin();
  for (it = objects.begin(); it != objects.end(); ++it) {
    it->print();
    if (it + 1 != objects.end())
      cout << endl;
  }
  return 0;
}

int parse_monster_templates(void **monsters, char *file) {
  string input_file(file);
  *monsters = (void *)new std::vector<monster>;
  parse_monsters(*((std::vector<monster> *)*monsters), input_file);
  return 0;
}

int parse_object_templates(void **objects, char *file) {
  string input_file(file);
  *objects = (void *)new std::vector<object>;
  parse_objects(*((std::vector<object> *)*objects), input_file);
  return 0;
}

void print_monster_templates(void *monsters) {
  print_monsters(*((std::vector<monster> *)monsters));
}

void print_objec_templates(void *objects) {
  print_objects(*((std::vector<object> *)objects));
}

void destroy_monster_templates(void *monsters){
  delete ((std::vector<monster> *)monsters);

}

void destroy_object_templates(void *objects){
  delete ((std::vector<object> *)objects);

}
