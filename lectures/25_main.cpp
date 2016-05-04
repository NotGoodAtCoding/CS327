#include <cstdio>

#include "string229.h"

int main(int argc, char *argv[])
{
  string229 s("Hello 229!");
  string229 t;

  s += "  Have a nice spring break.";

  std::cout << s + "  In 3 minutes." << std::endl;

  s = "Welcome back";

  std::cout << s << std::endl;

  t = s;

  std::cout << t << std::endl;

  printf(s);

  return 0;
}
