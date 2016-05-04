#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int compare_int(void *v1, void *v2)
{
  int *i1, *i2;

  i1 = v1;
  i2 = v2;

  return *i1 - *i2;
}

void insertion_sort(void *v, int n, size_t s, int (*compare)(void *, void *))
{
  int i, j;
  void *t;
  char *a;

  t = malloc(s);

  a = v;

  for (i = 1; i < n; i++) {
    for (memcpy(t, a + (s * i), s), j = i - 1;
         j > -1 && compare(v + (s * j), t);
         j--) {
      memcpy(a + (s * (j + 1)),  a + (s * j), s);
    }
    memcpy(a + (s * (j + 1)), t, s);
  }
}

int main(int argc, char *argv[])
{
  int i;
  int a[] = { 5, 8, 2, 5, 3, 8, 0, 9, 1, 4, 6, 8, 7, 3, 8 };

  insertion_sort(a, sizeof (a) / sizeof (a[0]), sizeof (a[0]), compare_int);

  for (i = 0; i < sizeof (a) / sizeof (a[0]); i++)
    printf("%d ", a[i]);
  printf("\n");

  return 0;
}
