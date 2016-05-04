#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class exam_list {
  class exam_list_node {
    public:
    const char *data;
    exam_list_node *next;
    exam_list_node *previous;
    inline exam_list_node(const char *d, exam_list_node *n, exam_list_node *p) :
      data(d), next(n), previous(p)
    {
      if (next) {
        next->previous = this;
      }
      if (previous) {
        previous->next = this;
      }
    }
  };
  private:
  exam_list_node *head;
  exam_list_node *tail;
  public:
  exam_list() : head(0), tail(0) {}
  // write prototype for 2a here
  exam_list(const exam_list &el);
  ~exam_list();
  void insert_head(const char *d);
  void insert_tail(const char *d);
  void insert_sorted(const char *d);
  // write prototype for 2b here
  friend ostream &operator<<(ostream &o, const exam_list &el);
};

exam_list::exam_list(const exam_list &el)
{
  exam_list_node *cur, *ccur;

  head = tail = 0;

  for (ccur = 0, cur = el.head; cur; cur = cur->next) {
    ccur = new exam_list_node(cur->data, 0, ccur);
    if (!head) {
      head = ccur;
    }
  }
  tail = ccur;
}

void exam_list::insert_tail(const char *d)
{
  exam_list_node *tmp;

  tmp = new exam_list_node(d, 0, tail);
  tail = tmp;

  if (!head) {
    head = tmp;
  }
}

ostream &operator<<(ostream &o, const exam_list &el)
{
  exam_list::exam_list_node *n;

  for (n = el.head; n; n = n->next) {
    o << n->data << endl;
  }

  return o;
}

exam_list::~exam_list()
{
}

const char *&tale() {
  static const char *knight = "Launcelot";

  cout << "The tale of Sir " << knight << ".\n";

  return knight;
}

int main(int argc, char *argv[])
{
  cout << "Are you suggesting coconuts migrate?" << endl;

  cout << 4 << " shalt thou not count, nor either count thou " << 2
       << ", excepting that thou then proceed to " << 3
       << ".  Five is right out.\n";

  stringstream ss;

  ss << "Brave Sir Robin ran away." << endl;
  ss << "Bravely ran away away." << endl;

  printf(&ss.str()[26]);

  tale() = "Galahad";

  cout << (tale() = "Robin") << endl;

  exam_list l;

  l.insert_tail("Launcelot");
  l.insert_tail("Galahad");
  l.insert_tail("Robin");

  cout << l << endl;

  exam_list m(l);

  cout << m << endl;

  return 0;
}
