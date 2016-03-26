#include <stdio.h>
#include <stdlib.h>

unsigned sequence(unsigned i)
{
  if (i == 0) {
    return 1;
  }

  if (i & 1) {
    return sequence((i - 1) / 2);
  }

  return sequence((i - 2) / 2) + sequence(i / 2);
}

int main(int argc, char *argv[])
{
  unsigned i;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <index>\n", argv[0]);
    return -1;
  }

  i = atoi(argv[1]);

  if (!i) {
    printf("0: 0\n");
  } else {
    printf("%d: %u/%u\n", i, sequence(i - 1), sequence(i));
  }

  return 0;
}
