#include <stdlib.h>
#include <string.h>

typedef struct q_element{
  void *data;
  int priority;
}q_element;

typedef struct p_queue{
  size_t data_size;
  int num;
  int capacity;
  q_element *items;
}p_queue;

void init_q(p_queue *q, size_t size);

void push(p_queue *q, void *data, int priority);
  
int pop(p_queue *q, void *to_return);

void bubble(p_queue *q, int start);

void trickle(p_queue *q, int start);

void destroy(p_queue *q);