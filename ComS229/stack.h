#ifndef _STACK_H
#define _STACK_H

#include "object_c.h"

typedef struct stack_node_s{
  struct stack_node_s *next;
  object_t *data;
}stack_node;

typedef struct obj_stack_s{
  int num_items;
  stack_node *head;
  stack_node *tail;
  size_t element_size;
}obj_stack_t;

void obj_stack_init(obj_stack_t *s, size_t element_size);

int obj_stack_push(obj_stack_t *s, object_t *data);

int obj_stack_pop(obj_stack_t *s, object_t *data);

object_t * obj_stack_peep(obj_stack_t *s);

void obj_stack_empty(obj_stack_t *s);

int obj_stack_size(obj_stack_t *s);

#endif
