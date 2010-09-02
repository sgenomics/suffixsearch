#ifndef SUFFIX2_OBJECTSTORE
#define SUFFIX2_OBJECTSTORE

#include <vector>

using namespace std;

/*
class Simple {
public:
 int val1;
 int val2;
 int val3;

};

class Complex {
public:
 Simple s1;
 int vala;
 };
*/

template<class object_type>
class ObjectStore {

private:

  size_t current_max;
  size_t object_size;
  vector<char> storage_area;

  ObjectStore() : current_max(0) {
  }

  static ObjectStore<object_type> *m_instance;

public:

  void initialise(size_t storage_size) {
    object_size = sizeof(object_type);
    storage_area = vector<char>(object_size*storage_size);
  }

  static ObjectStore<object_type> &get_instance() {
    if(m_instance == 0) m_instance = new ObjectStore();
    return *m_instance;
  }

  void set(size_t index,const object_type &o) {
    
    for(size_t n=0;n<object_size;n++) {
      storage_area[(index*object_size)+n] = ((const char *)(&o))[n];
    }
  }

  object_type get(size_t index) {
    object_type o;

    for(size_t n=0;n<object_size;n++) {
      ((char *)(&o))[n] = storage_area[(index*object_size)+n];
    }

    return o;
  }

  void push_back(const object_type &o) {
    size_t addr = add();
    set(addr,o);
  }

  size_t add() {
    current_max++;
    set(current_max-1,object_type());
    return current_max-1;
  }

  size_t get_max() {
    return current_max;
  }

};

template<class object_type> ObjectStore<object_type>* ObjectStore<object_type>::m_instance=0;

/*
int main() {

  Simple s;

  s.val1 = 1;
  s.val2 = 2;
  s.val3 = 3;

  ObjectStore<Simple> store(20);

  store.set(0,s);

  Simple s2 = store.get(0);

  cout << "val1: " << s2.val1 << endl;
  cout << "val2: " << s2.val2 << endl;
  cout << "val3: " << s2.val3 << endl;

  ObjectStore<Complex> store1(20);

  Complex c1;
  c1.s1 = s;
  c1.vala = 4;

  store1.set(0,c1);

  Complex c2;
  c2 = store1.get(0);

cout << "sizeof c1: " << sizeof(c1) << endl;
  cout << "c1 val1: " << c2.s1.val1 << endl;
  cout << "c1 val2: " << c2.s1.val2 << endl;
  cout << "c1 val3: " << c2.s1.val3 << endl;
  cout << "c1 vala: " << c2.vala << endl;
}
*/

#endif
