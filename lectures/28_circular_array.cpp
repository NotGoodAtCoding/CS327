#include <iostream>
#include <vector>

using namespace std;

template <class T>
class circular_queue {
public:
  vector<T> v;
  int size;
  int front, back;

  int enqueue(T d) {
    if (front != back) {
      v[front++] = d;
      if (front == size) {
        front = 0;
      }
      return 0;
    }
    throw "No space left in queue";
  };

  T dequeue() {
    if (((back + 1) % size) != front) {
      back++;
      if (back == size) {
        back = 0;
      }
    } else {
      throw "Nothing to dequeue";
    }

    return v[back];
  }

  circular_queue(int size) : v(size + 1), size(size + 1), front(0), back(size) {
  }

  ~circular_queue() {
  }
  
};

int main(int argc, char *argv[])
{
  circular_queue<int> q(10);

  cout << q.enqueue(1) << endl;
  cout << q.enqueue(2) << endl;
  cout << q.enqueue(3) << endl;
  cout << q.enqueue(4) << endl;
  cout << q.enqueue(5) << endl;
  cout << q.enqueue(6) << endl;
  cout << q.enqueue(7) << endl;
  cout << q.enqueue(8) << endl;
  cout << q.enqueue(9) << endl;
  cout << q.enqueue(10) << endl;

  cout << q.dequeue() << endl;
  cout << q.dequeue() << endl;
  cout << q.enqueue(11) << endl;
  cout << q.enqueue(12) << endl;
  try {
    cout << q.enqueue(13) << endl;
  }

  catch (const char *e) {
    cout << e << endl;
    throw;
  }

  circular_queue<string> qs(3); 
  cout << qs.enqueue("one") << endl;
  cout << qs.enqueue("two") << endl;
  cout << qs.enqueue("three") << endl;
  cout << qs.dequeue() << endl;
  cout << qs.dequeue() << endl; 

  return 0;
}
