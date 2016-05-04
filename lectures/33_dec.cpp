#include <iostream>

using namespace std;

class shape {
public:
  virtual ~shape() {}
  virtual void draw() = 0;
};

class square : public shape{
public:
  void draw()
  {
    cout << "[]" << endl;
  }
};

class circle : public shape {
public:
  void draw()
  {
    cout << "()" << endl;
  }
};

class shape_decorator : public shape{
private:
  shape *s;
public:
  shape_decorator(shape *s) : s(s) {}
  ~shape_decorator()
  {
    delete s;
  }
  void draw()
  {
    s->draw();
  }
};

class green_shape : public shape_decorator {
public:
  green_shape(shape *s) : shape_decorator(s) {}
  void draw()
  {
    cout << "GREEN-";
    shape_decorator::draw();
  }
};

class big_shape : public shape_decorator {
public:
  big_shape(shape *s) : shape_decorator(s) {}
  void draw()
  {
    cout << "BIG-";
    shape_decorator::draw();
  }
};

int main(int argc, char *argv[])
{
  square s;
  circle c;

  s.draw();
  c.draw();

  green_shape *gs = new green_shape(new circle());
  gs->draw();
  gs = new green_shape(new square());
  gs->draw();

  big_shape *bs = new big_shape(new green_shape(new circle()));

  bs->draw();

  big_shape *bbs = new big_shape(bs);
  bbs->draw();

  return 0;
}
