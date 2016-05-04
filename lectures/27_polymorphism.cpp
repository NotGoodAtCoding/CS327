#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

class shape {
public:
  // Need "virtual" keyword to get dynamic dispatch.
  virtual string name() { return string("shape"); }

  virtual double area() = 0;
  virtual double perimeter() = 0;
};

class square : public shape {
  double side;
public:
  // Initialization list initializes class member variables and superclasses
  square(double s) : side(s) {}
  string name() { return string("square"); }
  double area() { return side * side; }
  double perimeter() { return 4 * side; }
  double get_side() { return side; }
};

class circle : public shape {
  double radius;

public:
  circle(double r) : radius(r) {}
  string name() { return string("circle"); }
  double area() { return radius * radius * M_PI; }
  double perimeter() { return 2 * radius * M_PI; }
  double get_radius() { return radius; }
};

int main(int argc, char *argv[])
{
  square sq(2);
  circle c(1);
  int i;

  vector<shape *> v;
  vector<shape *>::iterator vi;

  shape *shp = &sq;
  square *sqp = &sq;

  cout  << sq.name() << " " << c.name() << endl;

  cout << shp->name() << endl;
  cout << sqp->name() << endl;

  cout << sq.perimeter() << " " << c.area() << endl;

  cout << shp->area() << endl;

  shape &sh = sq;

  cout << sh.area() << endl;

  for (i = 0; i < 5; i++) {
    v.push_back(new circle(i));
    v.push_back(new square(i));
  }

  for (vi = v.begin(); vi != v.end(); vi++) {
    if (dynamic_cast<square *>(*vi)) {
      cout << "square:" << dynamic_cast<square *>(*vi)->get_side() << endl;
    } else {
      cout << "circle:" << dynamic_cast<circle *>(*vi)->get_radius() << endl;
    }
  }

  return 0;
}
