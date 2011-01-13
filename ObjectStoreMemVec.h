#ifndef SUFFIX4_OBJECTSTOREMEMVEC
#define SUFFIX4_OBJECTSTOREMEMVEC

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdint.h>

using namespace std;

template<class object_type>
class ObjectStoreMemVec {

private:

  size_t object_size;
  vector<object_type> storage_area;

public:
  size_t current_max;
  typedef object_type value_type;

  ObjectStoreMemVec(size_t storage_size = 50) {
  }

  void set(size_t index,const object_type &o) {

    while(index < storage_area.size()) {
      storage_area.push_back(object_type());
    }

    storage_area[index] = o;
  }

  void clear() {
    storage_area.clear();
  }

  object_type get(size_t index) {
    return storage_area[index];
  }

  size_t push_back(const object_type &o) {
    storage_area.push_back(o);
    return storage_area.size()-1;
  }

  size_t get_max() {
    return storage_area.size();
  }

  size_t size() {
    return get_max();
  }

};

#endif
