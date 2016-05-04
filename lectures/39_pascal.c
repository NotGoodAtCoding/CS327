#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gmp.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char *argv[])
{
  mpz_t *row[2];
  unsigned i, j;
  char file[80];
  FILE *f;

  row[0] = calloc(1, sizeof (mpz_t));
  mpz_init(row[0][0]);
  mpz_set_ui(row[0][0], 1);

  row[1] = calloc(2, sizeof (mpz_t));
  mpz_init(row[1][0]);
  mpz_set_ui(row[1][0], 1);
  mpz_init(row[1][1]);
  mpz_set_ui(row[1][1], 1);

  for (i = 2;;i++) {
    row[i % 2] = realloc(row[i % 2], (i + 1) * sizeof (mpz_t));
    mpz_init(row[i % 2][i - 1]);
    mpz_init(row[i % 2][i]);
    mpz_set_ui(row[i % 2][0], 1);
    mpz_set_ui(row[i % 2][i], 1);
    for (j = 0; j < (i - 1); j++) {
      mpz_add(row[i % 2][j + 1], row[(i - 1) % 2][j], row[(i - 1) % 2][j + 1]);
    }
    if (!((i + 1) % 1000)) {
      sprintf(file, "pascals_triangle_row_%010d.txt", i + 1);
      f = fopen(file, "w");
      for (j = 0; j < i + 1; j++) {
        gmp_fprintf(f, "%Zd\n", row[i % 2][j]);
      }
      fclose(f);
    }
  }

  return 0;
}
