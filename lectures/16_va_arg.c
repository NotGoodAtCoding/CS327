#include <stdio.h>
#include <stdarg.h>
#include <math.h>

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
      f = va_arg(ap, double);
      printf("float %f\n", f);
      break;
    }
  va_end(ap);
}

#define eprintf(...) fprintf(stderr, ##__VA_ARGS__)

int main(int argc, char *argv[])
{
  foo("cddfs", 'g', 3, 5, M_PI, "Hello");

  eprintf("We just had an error\n");

  eprintf("We had %d errors!\n", 7000);

  return 0;
}

