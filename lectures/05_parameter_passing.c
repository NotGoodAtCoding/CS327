#include <stdio.h>
#include <stdlib.h>

void f(int *n)
{
  printf("Address of n inside is %p\n", n);
  *n = 10;
}

int r()
{
  return r();
}

int main(int argc, char *argv[])
{
  int *n;
  int i;

  n = malloc(sizeof (*n) * 5);

  for (i = 0; i < 5; i++) {
    n[i] = i;
  }
  for (i = 0; i < 5000000; i++) {
    printf("%d\n", n[i]);
  }

  printf("before, n is %d\n", n[0]); /* Indexing like an array is another 
                                      * way of dereferencing              */
  printf("Address of n outside is %p\n", n);
  f(n);
  printf("after, n is %d\n", *(n + 0));

  free(n);

  return 0;
}
