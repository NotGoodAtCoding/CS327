#include <stdlib.h>
#include <string.h>

#include "stack.h"

void obj_stack_init(obj_stack_t *s, size_t element_size){
  s->num_items = 0;
  s->head = s->tail = NULL;
  s->element_size = element_size;
}

int obj_stack_push(obj_stack_t *s, object_t *data){
  stack_node *new = malloc(sizeof(stack_node));
  new->data = malloc(sizeof(object_t));
  memcpy(new->data, data, sizeof(object_t));
  new->data->name = strdup(data->name);
  new->data->description = strdup(data->description);
  new->data->type = strdup(data->type);
  if(s->num_items == 0){
    s->head = s->tail = new;
  }else{
    new->next = s->head;
    s->head = new;
  }
  s->num_items++;
  return 0;
}

int obj_stack_pop(obj_stack_t *s, object_t *data){
  if(s->num_items <= 0){
    return -1;
  }
  memcpy(data, s->head->data, sizeof(object_t));
  data->name = strdup(s->head->data->name);
  data->description = strdup(s->head->data->description);
  data->type = strdup(s->head->data->type);
  destroy_obj(s->head->data);
  free(s->head->data);

  stack_node *to_delete = s->head;
  if(s->num_items == 1){
    s->tail = s->head = NULL;
  }else{
    s->head = s->head->next; 
  }
  free(to_delete);
  s->num_items--;
  return 0;
}

object_t * obj_stack_peep(obj_stack_t *s){
  if(s->num_items > 0){
    return s->head->data;
  }
  return NULL;
}

void obj_stack_empty(obj_stack_t *s){
  object_t temp;
  for(;s->num_items >=1; s->num_items--){
    obj_stack_pop(s, &temp);
    destroy_obj(&temp);
  }
}

void obj_stack_destroy(obj_stack_t *s){
  obj_stack_empty(s);
}

int obj_stack_size(obj_stack_t *s){
  return s->num_items;
}
