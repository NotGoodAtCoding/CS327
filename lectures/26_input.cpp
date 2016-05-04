#include <iostream> // For normal IO
#include <fstream>  // For file IO
#include <string>   // For strings
#include <sstream>  // For stringstreams
#include <vector>   // For vectors (duh!)

int main(int argc, char *argv[])
{
  std::ifstream f("input_file.txt");;
  std::string s;
  std::string::iterator i;
  int d;
  std::stringstream ss;
  std::vector<int> v;
  std::vector<int>::iterator vi;

  std::cout << (char) f.get(); /* get returns a single byte as an integer */
  std::cout << (char) f.get(); /* Cast to char so that cout handles it    */
  std::cout << (char) f.get(); /* correctly.                              */
  std::cout << (char) f.get();
  std::cout << (char) f.get();
  std::cout << (char) f.get();
  std::cout << (char) f.get();

  std::cout << (char) f.peek();
  std::cout << (char) f.peek();
  std::cout << (char) f.peek();
  std::cout << (char) f.peek();
  std::cout << std::endl;

  std::getline(f, s);

  std::cout << s << std::endl;

  for (i = s.begin(); i != s.end(); i++) {
    std::cout << *i << ' ';
  }

  std::cout << std::endl;

  f >> s >> d;

  std::cout << s << d << std::endl;

  //  not valid!
  //  s << "this is concatenating to my string s" << std::endl;

  ss << "this is concatenating to my stringstream ss" << std::endl;

  std::cout << ss.str() << std::endl;

  for (d = 0; d < 1000; d++) {
    v.push_back(d);
  }

  std::cout << v[100] << " " << v[375] << std::endl;

  for (d = 0; d < v.size(); d++) {
    std::cout << v[d] << " ";
  }
  std::cout << std::endl;

  for (vi = v.begin(); vi != v.end(); vi++) {
    std::cout << *vi << " ";
  }
  std::cout << std::endl;

  return 0;
}
