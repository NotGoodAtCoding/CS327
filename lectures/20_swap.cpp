#include <iostream>

using namespace std;

void cswap(int *x, int *y)
{
  int t;

  t = *x;
  *x = *y;
  *y = t;
}

void cppswap(int &x, int &y)
{
  int t;

  t = x;
  x = y;
  y = t;
}

int main(int argc, char *argv[])
{
  int i, j;

  i = 10;
  j = 20;

  cswap(&i, &j);

  cout << i << ", " << j << endl;

  cppswap(i, j);

  cout << i << ", " << j << endl;

  return 0;
}

