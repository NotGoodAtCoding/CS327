class visitor {
public:
  virtual void visit_visitable_1(visitable_1 *);
  virtual void visit_visitable_2(visitable_2 *);
  //...
  protected Visitor();
};


class visitable {
  public ~visitable();
  virtual void accept(visitor &) = 0;
protected:
  visitable();
};

class visitable_1 : public visitable {
public:
  visitable_1();
  virtual void accept(visitor &v) { v.visit_visitable_1(this); }
};

class printing_visitor : public visitor {
  virtual void visit_visitable_1(visitable_1 *v) { cout << v << endl; }
  virtual void visit_visitable_2(visitable_2 *v) { cout << v << endl; }
};

int main(int argc, char *argv[])
{
  visitable_1 v1;
  printing_visitor pv;

  v1.accept(&pv);
}
