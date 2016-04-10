#include <stdlib.h>

#include "object_c.h"

/* items for object_linked_list */

typedef struct list_node_s{
  struct list_node_s *next;
  object_t *data;
}list_node_t;

typedef struct list_s{
  list_node_t *head;
  list_node_t *tail;
  int num_items;
  size_t item_size;
}object_list_t;

int object_list_init(object_list_t *l, size_t item_size);

int object_list_push(object_list_t *l, object_t *obj );

object_t *object_list_retrieve(object_list_t *l, int num);

int object_list_remove(object_list_t *l, int num, object_t *to_return);

int object_list_size(object_list_t *l);

int object_list_destroy(object_list_t *l);



/* items to make generic linked_list */
typedef struct gen_list_node_s{
  struct gen_list_node_s *next;
  void*data;
}gen_list_node_t;

typedef struct gen_list_s{
  gen_list_node_t *head;
  gen_list_node_t *tail;
  int num_items;
  size_t item_size;
}gen_list_t;

int gen_list_init(gen_list_t *l, size_t item_size);

int gen_list_push(gen_list_t *l, void *obj );

void *gen_list_retrieve(gen_list_t *l, int num);

int gen_list_remove(gen_list_t *l, int num, void *to_return);

int gen_list_size(gen_list_t *l);

int gen_list_destroy(gen_list_t *l);


