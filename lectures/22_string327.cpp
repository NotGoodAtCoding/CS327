#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "string327.h"

string327::string327()
{
  str = (char *) malloc(1);
  str[0] = '\0';
}

string327::string327(const char *s)
{
  str = (char *) malloc(strlen(s) + 1);
  strcpy(str, s);
}

string327::~string327()
{
  free(str);
  str = NULL;
}

std::ostream &operator<<(std::ostream &o, const string327 &str)
{
  return o << str.c_str();
}

std::istream &operator>>(std::istream &i, const string327 &str)
{
  // Buffer overrun error.
  // istream::get() -> i.get() which returns a single character at a time
  return i >> str.str;
}

string327 string327::operator+(const string327 &s)
{
  string327 out;

  out.str = (char *) malloc(strlen(str) + strlen(s.str) + 1);
  sprintf(out.str, "%s%s", str, s.str);

  return out;
}

string327 &string327::operator+=(const string327 &s)
{
  str = (char *) realloc(str, strlen(str) + strlen(s.str) + 1);
  strcat(str, s.str);

  return *this;
}

string327 &string327::operator=(const char *s)
{
  free(str);

  str = strdup(s);

  return *this;
}

string327 &string327::operator=(const string327 &s)
{
  free(str);

  str = strdup(s.str);

  return *this;
}
