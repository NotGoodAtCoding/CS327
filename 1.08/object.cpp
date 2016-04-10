//
// Created by Stefan Kraus on 3/29/2016.
//

#include "utils.h"
#include "object.h"
#include "dims.h"
#include "dungeon.h"

object::object(void){}

void gen_objects(dungeon_t *d) {
    uint32_t i;
    object *o;
    uint32_t room;
    pair_t p;

    for (i = 0; i < d->object_descriptions.size(); i++) {
        o = new object();

        do {
            room = rand_range(1, d->num_rooms - 1);
            p[dim_y] = rand_range(d->rooms[room].position[dim_y],
                                  (d->rooms[room].position[dim_y] +
                                   d->rooms[room].size[dim_y] - 1));
            p[dim_x] = rand_range(d->rooms[room].position[dim_x],
                                  (d->rooms[room].position[dim_x] +
                                   d->rooms[room].size[dim_x] - 1));
        } while (d->objectmap[p[dim_y]][p[dim_x]]);
        o->position[dim_y] = p[dim_y];
        o->position[dim_x] = p[dim_x];
        d->objectmap[p[dim_y]][p[dim_x]] = (object *) o;

        o->name = d->object_descriptions[i].get_name();
        o->description = d->object_descriptions[i].get_description();
        o->symbol = (char)d->object_descriptions[i].get_type();
        o->color = d->object_descriptions[i].get_color();
        o->hit_bonus = d->object_descriptions[i].get_hit().roll();
        o->dam_bonus = d->object_descriptions[i].get_damage();
        o->dge_bonus = d->object_descriptions[i].get_dodge().roll();
        o->def_bonus = d->object_descriptions[i].get_defence().roll();
        o->weight = d->object_descriptions[i].get_weight().roll();
        o->spd_bonus = d->object_descriptions[i].get_speed().roll();
        o->spec_attr = d->object_descriptions[i].get_attribute().roll();
        o->value = d->object_descriptions[i].get_value().roll();
    }
}

int8_t *object_get_pos(const object *o){
    return ((object *) o)->position;
}

uint32_t object_get_color(const object *o){
    return ((object *) o)->color;
}

char object_get_symbol(const object *o){
    return ((object *) o)->symbol;
}
