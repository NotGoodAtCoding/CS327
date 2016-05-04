#include <stdio.h>

#include "stack.h"

int main(int argc, char *argv[])
{
  int i;
  stack_t s;

  stack_init(&s);

  for (i = 0; i < 100; i++) {
    stack_push(&s, i);
  }

  printf("Stack contains %d elements\n", s.size);

  while (i > 50) {
    stack_pop(&s, &i);
    printf("Stack contains %d elements\n", s.size);
    printf("Value is %d\n", i);
  }

  printf("Stack contains %d elements\n", s.size);
  stack_delete(&s);
  printf("Stack contains %d elements\n", s.size);

  return 0;
}
