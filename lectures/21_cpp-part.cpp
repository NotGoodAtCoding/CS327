#include <iostream>

#include "header.h"

void cpp_function(const char *s)
{
  std::cout << s << std::endl;
}

void *get_cout(void)
{
  return (void *) &std::cout;
}

void use_cout(void *ptr)
{
  (*(std::ostream *)ptr) << "Hello!\n";
}
