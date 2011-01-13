#ifndef SUFFIX4_OBJECTSTOREMEM
#define SUFFIX4_OBJECTSTOREMEM

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdint.h>

using namespace std;

template<class object_type>
class ObjectStoreMem {

private:

  size_t object_size;
  vector<char> storage_area;

public:
  size_t current_max;
  typedef object_type value_type;

  ObjectStoreMem(size_t storage_size = 50) : current_max(0) {
    initialise(storage_size);
  }

  void initialise(size_t storage_size) {
    object_size = sizeof(object_type);
    size_t total_size = object_size*storage_size;
    cout << "Object store allocating: " << total_size << endl;
    storage_area = vector<char>(total_size);
  }

  void operator=(ObjectStoreMem<object_type> &other) {
    object_size  = other.object_size;
    storage_area = other.storage_area;
    current_max  = other.current_max;
  }

  void set(size_t index,const object_type &o) {

//cout << "MAGIC";
    size_t write_end_position = (index+2)*(object_size);

    bool expanding = false;
    if(write_end_position > storage_area.size()) {
      expanding = true;
    }
    for(;write_end_position > storage_area.size();) {
      storage_area.push_back(0);
    }
//    if(expanding) cout << "Storage area nsize: " << storage_area.size() << endl;

    //cout << "object size is: " << object_size << endl;
    const char *base_pointer = reinterpret_cast<const char *> (&o);
    for(size_t n=0;n<object_size;n++) {

      uint8_t c = ((const char *) (base_pointer+n))[0]; //((const char *)(&o))[n];
 //     cout << n << " val: " << (size_t) c;
      storage_area[(index*object_size)+n] = c;
    }
  }

  void clear() {
    current_max = 0;
    storage_area.clear();
    initialise(0);
  }

  object_type get(size_t index) {
    // it would be nice to remove this new and allocate on the stack
    char *o = new char[sizeof(object_type)];

    if(index > size()) {
      cout << "error trying to get an object that's out of bounds, index is: " << index << endl;
      int *i=0;*i=1;
    }

    char *base_pointer = o;
    size_t object_size = sizeof(object_type);
    size_t base_read = index*object_size;
    for(size_t n=0;n<object_size;n++) {
      *(base_pointer+n) = storage_area[base_read+n];
  //    cout << "str: " << storage_area[base_read+n] << " ";
   //MAGIC   
 // cout << n << " val: " << (size_t) (base_pointer+n) << endl;
    }

    object_type oo = *(reinterpret_cast<object_type *>(o));

    delete [] o;

    return oo;
  }

  size_t push_back(const object_type &o) {
    //cout << "objectstore pushback" << endl;
    //   size_t addr = add();

    set(current_max,o);
    current_max++;
    return current_max-1;
  }
/*
  size_t add() {
    current_max++;
    set(current_max-1,object_type());
    return current_max-1;
  }
*/
  size_t get_max() {
    return current_max;
  }

  size_t size() {
    return get_max();
  }

};

// template<class object_type> ObjectStoreMem<object_type>* ObjectStoreMem<object_type>::m_instance=0;

#endif
