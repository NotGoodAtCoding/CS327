#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#define eprintf(...) fprintf(stderr, ##__VA_ARGS__)

/* Example code taken directly from stdarg(3) */
void foo(char *fmt, ...)
{
  va_list ap;
  int d;
  char c, *s;
  float f;

  va_start(ap, fmt);
  while (*fmt)
    switch (*fmt++) {
    case 's':              /* string */
      s = va_arg(ap, char *);
      printf("string %s\n", s);
      break;
    case 'd':              /* int */
      d = va_arg(ap, int);
      printf("int %d\n", d);
      break;
    case 'c':              /* char */
      /* need a cast here since va_arg only
         takes fully promoted types */
      c = (char) va_arg(ap, int);
      printf("char %c\n", c);
      break;
    case 'f':
      f = (float) va_arg(ap, double);
      printf("float %f\n", f);
      break;
    }
  va_end(ap);
}

int main(int argc, char *argv[])
{
  foo("sdcdcsdscf", "Phineas", 2, 'd', 3, 'g', "Ferb", 5, "Isabella", 'q', M_PI);

  eprintf("This is an %s!\n", "error");

  return 0;
}
