#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int compare_int(const void *v1, const void *v2)
{
  const int *i1, *i2;

  i1 = v1;
  i2 = v2;

  return *i1 - *i2;
}

/* Void pointers can't be dereferenced, partially because the compiler *
 * doesn't know how big the data they address is.  We need to pass a   *
 * comparitor that does the dereference for us internally, but the     *
 * generic function needs to set up the pointers for us, so we also    *
 * need to explicitly pass the size of each item.                      */
void insertion_sort(void *v, int n, size_t s,
                    int (*compare)(const void *, const void *))
{
  int i, j;
  void *t;
  char *a;

  t = malloc(s);

  /* Assign v to a character pointer because pointer arithmetic.  *
   * Remember that adding 1 to a pointer increments that pointer  *
   * by sizeof (addressed type) bytes.  Since we are passing in a *
   * precise number of bytes per element, the only way we can     *
   * get the pointer to behave as we need when we add to it is if *
   * it addressed a one byte type, char (the single byte stdint   *
   * types, like uint8_t and int8_t, would also do the trick).    */
  a = v;

  /* Compare this to the integer insertion sort from the notes.  *
   * Same algorithm; only the mechanics have changed.            */
  for (i = 1; i < n; i++) {
    for (memcpy(t, a + (s * i), s), j = i - 1;
         j > -1 && compare(v + (s * j), t) > 0;
         j--) {
      memcpy(a + (s * (j + 1)),  a + (s * j), s);
    }
    memcpy(a + (s * (j + 1)), t, s);
  }

  free(t);
}

/* What come in to this function are pointers to pointers to char, which is *
 * why can can't pass strcmp() directly.  This wrapped simply does the      *
 * outermost dereference for us, then calls strcmp on that and returns the  *
 * result.                                                                  */
int strpcmp(const void *v1, const void *v2)
{
  return strcmp(*(const char **) v1, *(const char **) v2);
}

int main(int argc, char *argv[])
{
  int i;
  int a[] = { 5, 8, 2, 5, 3, 8, 0, 9, 1, 4, 6, 8, 7, 3, 8 };
  char *s[] = {
    "one", "two", "three", "four", "five"
  };

  insertion_sort(a, sizeof (a) / sizeof (a[0]), sizeof (a[0]), compare_int);

  for (i = 0; i < sizeof (a) / sizeof (a[0]); i++)
    printf("%d ", a[i]);
  printf("\n");

  /* Comment out insertion sort and uncomment qsort to see qsort do the *
   * same thing. */
  insertion_sort(s, sizeof (s) / sizeof (s[0]), sizeof (s[0]), strpcmp);

  /*
  qsort(s, 5, 8, (void *) strpcmp);
  */

  for (i = 0; i < sizeof (s) / sizeof (s[0]); i++)
    printf("%s ", s[i]);
  printf("\n");

  return 0;
}
