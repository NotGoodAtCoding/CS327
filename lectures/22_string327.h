#ifndef STRING327_H
# define STRING327_H

# include <iostream>
# include <cstring> // Equivalent to string.h in C

class string327 {
  char *str;
 public:
  string327();
  ~string327();
  string327(const char *s);
  inline int length() const
  {
    return strlen(str);
  }
  inline const char *c_str() const
  {
    return str;
  }

  // Users don't worry about string length
  // Overload comparison operators
  inline int operator<(const string327 &s)
  {
    return strcmp(str, s.str) < 0;
  }
  inline int operator<=(const string327 &s)
  {
    return strcmp(str, s.str) <= 0;
  }
  inline int operator>(const string327 &s)
  {
    return strcmp(str, s.str) > 0;
  }
  inline int operator>=(const string327 &s)
  {
    return strcmp(str, s.str) >= 0;
  }
  inline int operator==(const string327 &s)
  {
    return strcmp(str, s.str) == 0;
  }
  inline int operator!=(const string327 &s)
  {
    return strcmp(str, s.str) != 0;
  }

  // Append using +, +=
  string327 operator+(const string327 &s);
  string327 &operator+=(const string327 &s);
  // Copy with assignment statement
  string327 &operator=(const char *s);
  string327 &operator=(const string327 &s);
  // Get individual characters with [].  Maybe modify?
  inline char &operator[](const int index)
  {
    return str[index];
  }

  inline operator const char *()
  {
    return str;
  }
  // Output with <<
  // Read with >>
  friend std::istream &operator>>(std::istream &i, const string327 &str);
};

std::ostream &operator<<(std::ostream &o, const string327 &str);

#endif

