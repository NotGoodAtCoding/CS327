#ifndef STACK_H
# define STACK_H

typedef struct stack_node {
  struct stack_node *prev;
  int value;
} stack_node_t;

typedef struct stack {
  stack_node_t *head;
  int size;
} stack_t;

int stack_init(stack_t *s);
int stack_delete(stack_t *s);
int stack_push(stack_t *s, int v);
int stack_pop(stack_t *s, int *v);

#endif
