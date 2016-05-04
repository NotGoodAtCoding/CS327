#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>

using namespace std;

class greater_than
{
private:
  int value;
public:
  greater_than(int i) : value(i) {}
  bool operator()(int i) { return i > value; }
};

void print_int(int i)
{
  cout << i << endl;
}

int main(int argc, char *argv[])
{
  greater_than gt65(65);
  cout << gt65(4) << " " << gt65(70) << endl;

  vector<int> v;
  vector<int>::iterator vi;
  int i;
  
  for (i = 0; i < 23; i++) {
    v.push_back(rand() % 100);
  }

  for (i = 0; i < 23; i++) {
    cout << v[i] << endl;
  }

  for (vi = v.begin(); vi != v.end(); vi++) {
    cout << *vi << endl;
  }

  vi = v.begin();
  vi++;
  vi++;
  vi++;

  sort(vi, v.end());
  for (vi = v.begin(); vi != v.end(); vi++) {
    cout << *vi << endl;
  }

  vi = find(v.begin(), v.end(), 93);

  if (vi == v.end()) {
    cout << "Item not found" << endl;
  } else {
    cout << *vi << endl;
  }

  cout << "-------------------------------------" << endl;
  for (vi = v.begin(); (vi = find_if(vi, v.end(), gt65)) != v.end(); vi++) {
    cout << *vi << endl;
  }
  cout << "-------------------------------------" << endl;
  for_each(v.begin(), v.end(), print_int);

  return 0;
}
