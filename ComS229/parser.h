#ifndef PARSER_H
#define PARSER_H

#define object_descripter_key "RLG229 OBJECT DESCRIPTION 1"
#define object_begin_key "BEGIN OBJECT"

#define monster_descripter_key "RLG229 MONSTER DESCRIPTION 1"
#define monster_begin_key "BEGIN MONSTER"
#define end_key "END"

#define NAME "NAME"
#define SYMBOL "SYMB"
#define COLOR "COLOR"
#define DESCRIPTION "DESC"
#define ABILITY "ABIL"
#define SPEED "SPEED"
#define DAMAGE "DAM"
#define HEALTH "HP"

#define TYPE "TYPE"
#define HIT "HIT"
#define DODGE "DODGE"
#define DEFENSIVE "DEF"
#define WEIGHT "WEIGHT"
#define ATTRIBUTE "ATTR"
#define VALUE "VAL"

#ifdef __cplusplus
extern "C" {
#endif

  int parse_monster_templates(void **monsters, char *file);

  void print_monster_templates(void *monsters);

  void destroy_monster_templates(void *monsters);

  int parse_object_templates(void **objects, char *file);

  void print_object_templates(void *objects);

  void destroy_object_templates(void *objects);

#ifdef __cplusplus
}
#endif

#endif
