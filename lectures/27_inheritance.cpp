#include <iostream>
#include <math.h>
#include <vector>

class shape {
public:
  virtual double area() = 0;
  virtual double perimeter() = 0;
};

class square : public shape {
public:
  double side;
  square(double s) : side(s)
  {
  }
  double area()
  {
    return side * side;
  }
  double perimeter()
  {
    return 4 * side;
  }
};

class circle : public shape {
public:
  double radius;
  circle(double r) : radius(r)
  {
  }
  double area()
  {
    return M_PI * radius * radius;
  }
  double perimeter()
  {
    return 2 * radius * M_PI;
  }
};

int main(int argc, char *argv[])
{
  shape *sp;
  square s(4);
  circle c(2);
  std::vector<shape *> v;
  std::vector<shape *>::iterator i;

  sp = &s;

  std::cout << sp->area() << " " << s.area() << " " << c.area() << std::endl;

  sp = &c;

  std::cout << sp->area() << " " << s.area() << " " << c.area() << std::endl;

  shape *spp;
  spp = &c;

  std::cout << spp->area() << " " << s.area() << " " << c.area() << std::endl;

  shape &spr = c;

  std::cout << spr.area() << " " << s.area() << " " << c.area() << std::endl;

  v.push_back(&s);
  v.push_back(&c);

  for (i = v.begin(); i != v.end(); i++) {
    std::cout << (*i)->perimeter() << " ";

    if (dynamic_cast<square *>(*i)) {
      std::cout << "Side: " << dynamic_cast<square *>(*i)->side << std::endl;
    } else {
      std::cout << "Radius: " << dynamic_cast<circle *>(*i)->radius << std::endl;
    }
  }

  return 0;
}
