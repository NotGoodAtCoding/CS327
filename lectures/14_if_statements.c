#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void zero(int i)
{
  printf("%d is spelled zero\n", i);
}
void one(int i)
{
  printf("%d is spelled one\n", i);
}
void two(int i)
{
  printf("%d is spelled two\n", i);
}
void three(int i)
{
  printf("%d is spelled three\n", i);
}

typedef void (*func)(int);

func spelling_jump_table[4] = {
  zero,
  one,
  two,
  three
};

int main(int argc, char *argv[])
{
  int i;

  srand(time(NULL));

  i = rand() % 4;

  spelling_jump_table[i](i);

  for (;;);

  return 0;
}
