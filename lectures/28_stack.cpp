#include <iostream>

using namespace std;

template <class T>
class stack {
  class node {
  public:
    node *next;
    T val;
    node(T i) : next(0), val(i) {}
  };

public:
  node *head;

  stack() { head = 0; }

  void push(T i) {
    node *n;

    n = new node(i);

    n->next = head;
    head = n;
  }

  T pop() {
    node *n = head;
    T i;

    if (!n) {
      throw "Attempt to pop an empty stack";
    }
    i = n->val;
    head = n->next;
    delete n;

    return i;
  }

};

template <class T>
T min(T &t1, T &t2)
{
  if (t1 < t2) {
    return t1;
  }
  return t2;
}

int main(int argc, char *argv[])
{
  stack<int> s, t;
  stack<char *> c;
  int i;

  s.push(0);
  s.push(1);
  s.push(2);
  s.push(3);
  s.push(4);

  c.push("A");
  c.push("B");
  c.push("C");
  c.push("D");
  c.push("E");

  try{
    for (i = 0; i < 6; i++) {
      cout << s.pop() << endl;
      cout << c.pop() << endl;
    }
  }
  /*
  catch (char const *s) {
    cout << s << endl;
  }
  */
  catch (...) {
    cout << "I caught an exception!" << endl;
    throw;
  }
  return 0;
}
