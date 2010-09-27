#ifndef SUFFIX4_OBJECTSTORE
#define SUFFIX4_OBJECTSTORE

#include <vector>

using namespace std;

template<class object_type>
class ObjectStore {

private:

  size_t current_max;
  size_t object_size;
  vector<char> storage_area;

public:

  ObjectStore(size_t storage_size = 50000) : current_max(0) {
    initialise(storage_size);
  }

//  static ObjectStore<object_type> *m_instance;

  void initialise(size_t storage_size) {
    object_size = sizeof(object_type);
    storage_area = vector<char>(object_size*storage_size);
  }

 /* static ObjectStore<object_type> &get_instance() {
    if(m_instance == 0) m_instance = new ObjectStore();
    return *m_instance;
  }
  */

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

  size_t push_back(const object_type &o) {
    size_t addr = add();
    set(addr,o);

    return addr;
  }

  size_t add() {
    current_max++;
    set(current_max-1,object_type());
    return current_max-1;
  }

  size_t get_max() {
    return current_max;
  }

  size_t size() {
    return get_max();
  }

};

// template<class object_type> ObjectStore<object_type>* ObjectStore<object_type>::m_instance=0;

#endif
