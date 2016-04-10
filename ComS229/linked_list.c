#include "linked_list.h"

#include <string.h>

int object_list_init(object_list_t *l, size_t item_size){
  l->item_size = item_size;
  l->num_items = 0;
  l->head = l->tail = NULL;
  return 0;
}

int object_list_push(object_list_t *l, object_t *obj){
  list_node_t *new = malloc(sizeof(list_node_t));
  new->data = malloc(l->item_size);
  new->next = NULL;
  memcpy(new->data, obj, l->item_size);
  new->data->name = strdup(obj->name);
  new->data->description = strdup(obj->description);
  new->data->type = strdup(obj->type);
  
  // if first item
  if(l->num_items == 0){
    l->head = l->tail = new;
  }else{
    l->tail->next = new;
    l->tail = new;
  }
  l->num_items++;
  return 0;
}

object_t *object_list_retrieve(object_list_t *l, int num){
  if(num == 0)
    return (l->head->data);
  else if(num <= l->num_items-1){
      int i;
      list_node_t *itr = l->head;
       for(i = 0; i < num; i++, itr = itr->next ){
         ;
       }
       return (itr->data);
    
  }
  return NULL;
	
}

int object_list_remove(object_list_t *l, int num, object_t *to_return){
  int i;
  list_node_t *itr = l->head;
  if(num > l->num_items-1)
    return -1;
  if(num == 0){
    l->head = l->head->next;
    memcpy(to_return, itr->data, l->item_size);
    to_return->name = strdup(itr->data->name);
    to_return->description = strdup(itr->data->description);
    to_return->type = strdup(itr->data->type);
    destroy_obj(itr->data);
    free(itr->data);
    free(itr);
    l->num_items--;
    return 0;
  }else{
    itr = l->head;
    for(i = 0; i < num-1; i++, itr = itr->next ){;}
    memcpy(to_return, itr->next->data, l->item_size);
    to_return->name = strdup( itr->next->data->name);
    to_return->description = strdup(itr->next->data->description);
    to_return->type = strdup( itr->next->data->type);
    list_node_t *to_delete = itr->next;
    itr->next = itr->next->next;
    if(l->tail == to_delete)
      l->tail = itr;
    destroy_obj(to_delete->data);
    free(to_delete->data);
    free(to_delete);
    l->num_items--;
      return 0;
  }

}

int object_list_destroy(object_list_t *l){
  list_node_t *itr = l->head;
  int i;
  for(i = 0; i < l->num_items; i++){
    free(itr->data->name);
    free(itr->data->description);
    free(itr->data->type);
    free(itr->data);
    list_node_t *to_delete = itr;
    itr = itr->next;
    free(to_delete);
  }
  return 0;
}

int object_list_size(object_list_t *l){
  return l->num_items;
}
/*****************

for generic linked_list

*****************/

int gen_list_init(gen_list_t *l, size_t item_size){
  l->item_size = item_size;
  l->num_items = 0;
  l->head = l->tail = NULL;
  return 0;
}

int gen_list_push(gen_list_t *l, void *obj ){
  gen_list_node_t *new = malloc(sizeof(gen_list_node_t));
  new->data = malloc(l->item_size);
  new->next = NULL;
  memcpy(new->data, obj, l->item_size);
  
  // if first item
  if(l->num_items == 0){
    l->head = l->tail = new;
  }else{
    l->tail->next = new;
    l->tail = new;
  }
  l->num_items++;
  return 0;
}

void *gen_list_retrieve(gen_list_t *l, int num){
  if(num == 0)
    return (l->head->data);
  else if(num <= l->num_items-1){
      int i;
      gen_list_node_t *itr = l->head;
       for(i = 0; i < num; i++, itr = itr->next ){
         ;
       }
       return (itr->data);
    
  }
  return NULL;
	
}

int gen_list_remove(gen_list_t *l, int num, void *to_return){
  int i;
  gen_list_node_t *itr = l->head;
  if(num > l->num_items-1)
    return -1;
  if(num == 0){
    l->head = l->head->next;
    memcpy(to_return, itr->data, l->item_size);
    free(itr->data);
    free(itr);
    l->num_items--;
    return 0;
  }else{
    itr = l->head;
    for(i = 0; i < num-1; i++, itr = itr->next ){;}
    memcpy(to_return, itr->next->data, l->item_size);
    gen_list_node_t *to_delete = itr->next;
    itr->next = itr->next->next;
    if(l->tail == to_delete)
      l->tail = itr;
    free(to_delete->data);
    free(to_delete);
    l->num_items--;
      return 0;
  }

}

int gen_list_size(gen_list_t *l){
  return l->num_items;
}

int gen_list_destroy(gen_list_t *l){
  gen_list_node_t *itr = l->head;
  int i;
  for(i = 0; i < l->num_items; i++){
    free(itr->data);
    gen_list_node_t *to_delete = itr;
    itr = itr->next;
    free(to_delete);
  }
  return 0;
}
