#include <stdio.h>
#include <stdlib.h>

#define MAX_VALUE 50000000

int seq[MAX_VALUE];

int main(int argc, char *argv[])
{
  int i;

  seq[0] = 1;

  printf("%d\n", seq[0]);
  for (i = 1; i < MAX_VALUE; i++) {
    if (i % 2) {
      seq[i] = seq[(i - 1) / 2];
    } else {
      seq[i] = seq[(i - 2) / 2] + seq[((i - 2) / 2) + 1];
    }
    printf("%d\n", seq[i]);
  }
  for (i = MAX_VALUE; i < MAX_VALUE * 2; i++) {
    if (i % 2) {
      printf("%d\n", seq[(i - 1) / 2]);
    } else {
      printf("%d\n", seq[(i - 2) / 2] + seq[((i - 2) / 2) + 1]);
    }
  }
}
