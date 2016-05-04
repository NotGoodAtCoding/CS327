#include "header.h"
#include "header2.h"

#define THIS_MACRO 5
#define ANOTHER_MACRO "This is a string"
#define print(x) printf("%d", x)

#define max(a, b) ({ \
  typeof (a) _a;     \
  typeof (b) _b;     \
  _a = (a);          \
  _b = (b);          \
  _a > _b ? _a : _b; \
})

int main(int argc, char *argv[])
{
  data_structure_t d;

  print(ANOTHER_MACRO);

  print(max(f(3), g(5)));

  return 0;
}
