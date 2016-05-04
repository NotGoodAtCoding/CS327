#include <iostream>
#include <cstring>
#include <cstdlib>

class singleton {
private:
  static singleton *the_instance;
  singleton() { i = 0; }
  singleton(const singleton &) { i = 0; }
  singleton &operator=(const singleton &) { return *the_instance; }
public:
  int i;
  static singleton *instance();
  int get_i() { return i; }
  friend singleton *break_singleton();
};

singleton *singleton::the_instance = 0;

singleton *singleton::instance()
{
  if (!the_instance) {
    the_instance = new singleton();
  }

  return the_instance;
}

singleton *break_singleton()
{
  return new singleton();
}

int main(int argc, char *argv[])
{
  singleton *s, *t;

  s = singleton::instance();
  t = singleton::instance();
  t = break_singleton();
  t->i = 1;

  t = (singleton *) malloc(sizeof (*t));
  memcpy(t, s, sizeof (*t));

  t->i = 2;

  std::cout << s << " " << s->i << s << " " << s->get_i() << std::endl;
  std::cout << t << " " << t->i << t << " " << t->get_i() << std::endl;

  return 0;
}
