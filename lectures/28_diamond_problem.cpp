#include <iostream>

using namespace std;


class A {
public:
  A() { cout << "constructing A" << endl; }
  virtual ~A() { cout << "destructing A" << endl; }
  virtual void print() { cout << "A" << endl; }
};

class B : public virtual A {
public:
  B() { cout << "constructing B" << endl; }
  virtual ~B() { cout << "destructing B" << endl; }
  virtual void print() { cout << "B" << endl; }
};

class C : public virtual A {
public:
  C() { cout << "constructing C" << endl; }
  virtual ~C() { cout << "destructing C" << endl; }
  virtual void print() { cout << "C" << endl; }
};

class D : public B, public C {
public:
  D() { cout << "constructing D" << endl; }
  ~D() { cout << "destructing D" << endl; }
  virtual void print() { cout << "C" << endl; }
};

int main(int argc, char *argv[])
{
  /*
  cout << "Instantiating A" << endl;
  A a;

  cout << "Instantiating B" << endl;
  B b;
  */

  cout << "Instantiating D" << endl;
  D *d = new D;

  d->A::print();

  return 0;
}
