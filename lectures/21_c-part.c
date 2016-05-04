#include "header.h"

int main(int argc, char *argv[])
{
  void *p;

  p = get_cout();
  use_cout(p);

  return 0;
}
