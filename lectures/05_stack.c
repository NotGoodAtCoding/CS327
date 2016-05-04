#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

int stack_init(stack_t *s)
{
  s->size = 0;
  s->head = NULL;

  return 0;
}

int stack_push(stack_t *s, int v)
{
  stack_node_t *n;

  if (!(n = malloc(sizeof (*n)))) {
    return 1;
  }
  n->value = v;
  n->prev = s->head;
  s->head = n;
  s->size++;

  return 0;
}

int stack_pop(stack_t *s, int *v)
{
  stack_node_t *n;

  if (!s->head) {
    return 1;
  }

  *v = s->head->value;
  n = s->head;
  s->head = s->head->prev;
  free(n);
  s->size--;

  return 0;
}

int stack_delete(stack_t *s)
{
  /* This works just fine!
  int i;

  while (!stack_pop(s, &i))
    ;
  */

  stack_node_t *n, *m;

  for (n = s->head; n; n = m) {
    m = n->prev;
    free(n);
  }

  s->head = NULL;
  s->size = 0;

  return 0;
}
