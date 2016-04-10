#include "p_queue.h"

void init_q(p_queue *q, size_t size){
  if(size <=0)
    exit (-1);
  q->data_size = size;
  q->num = 0;
  q->capacity = 100;
  q->items = malloc(sizeof(*q->items) * (q->capacity+1));
}

void push(p_queue *q, void *data, int priority){
  if(q->num == q->capacity){
    q_element *temp, *trash;
    int i;
    size_t old_size = q->capacity;
    q->capacity *=2;
    
    temp = malloc(sizeof(q_element) * (q->capacity+1));
    for(i = 1; i <= old_size; i++){
      temp[i] = q->items[i];
    }
    trash = q->items;
    free(trash);
    q->items=temp;
  }
  q->num++;  
  q->items[q->num].data = malloc(q->data_size); 
  memcpy(q->items[q->num].data, data, q->data_size);
  //q->items[q->num].data = data;
  q->items[q->num].priority = priority;

  
  //q_element *new = malloc(sizeof(q_element));

  //new->data = malloc(sizeof(q->data_size));
  //memcpy(new->data, data, q->data_size);
  //new->priority = priority;
  //q->num++;
//  q->items[q->num] = *new;
  bubble(q, q->num);
}

int pop(p_queue *q, void *to_return){
  if(q->num < 1)
    return 1;
  else{
    memcpy(to_return, q->items[1].data, q->data_size);
    free(q->items[1].data);
    //to_return = q->items[1].data;
    q->items[1] = q->items[q->num];
    q->num--;
    trickle(q, 1);
    return 0;
  }
}

void bubble(p_queue *q, int start){
  if(start > 1)
    if(q->items[start/2].priority > q->items[start].priority){
      q_element temp = q->items[start/2];
      q->items[start/2] = q->items[start];
      q->items[start] = temp;
      bubble(q, start/2);
    }
}

void trickle(p_queue *q, int start){
  if(q->num == start*2){
    if(q->items[start].priority > q->items[2*start].priority){
      q_element temp = q->items[start*2];
      q->items[start*2] = q->items[start];
      q->items[start] = temp;
    }
  }
  else if(2*start+1 <= q->num){
    int smallest;
    if(q->items[start*2].priority < q->items[start*2+1].priority)
      smallest = start*2;
    else
      smallest = start*2+1;
    if(q->items[start].priority > q->items[smallest].priority){
      q_element temp = q->items[start];
      q->items[start] = q->items[smallest];
      q->items[smallest] = temp;
      trickle(q, smallest);
    }
  }
}

void destroy(p_queue *q){
  void *temp = malloc(q->data_size);
  while(pop(q,temp) == 0);
  free(temp);
  free(q->items);
}
