#include <iostream>

using namespace std;

class function_like;

typedef function_like (*func)();

class function_like {
public:
  func operator()();
};

function_like function()
{
  return function_like();
}

func function_like::operator()()
{
  return (func) function;
}

class fl;

class f {
public:
  fl operator()();
};

class fl {
public:
  f operator()();
};

fl f::operator()()
{
  cout << __PRETTY_FUNCTION__ << endl;

  return fl();
}

f fl::operator()()
{
  cout << __PRETTY_FUNCTION__ << endl;

  return f();
}

class maximal_fun {
public:
  maximal_fun &operator()() {
    cout << __PRETTY_FUNCTION__ << endl;

    return *this;
  }
};

int main(int argc, char *argv[])
{

  function()()()()()()();

  f()()()()()();

  maximal_fun()()()()()()()()();

  return 0;
}
