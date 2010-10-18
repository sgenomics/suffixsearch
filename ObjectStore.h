#ifndef SUFFIX4_OBJECTSTORE
#define SUFFIX4_OBJECTSTORE

#include <vector>
#include <iostream>

using namespace std;

template<class object_type>
class ObjectStore {

private:

  size_t object_size;
  vector<char> storage_area;

public:
  size_t current_max;
  typedef object_type value_type;

  ObjectStore(size_t storage_size = 50) : current_max(0) {
    initialise(storage_size);
  }

  void initialise(size_t storage_size) {
    object_size = sizeof(object_type);
    size_t total_size = object_size*storage_size;
    cout << "Object store allocating: " << total_size << endl;
    storage_area = vector<char>(total_size);
  }

  void set(size_t index,const object_type &o) {

    //cout << "object store set: " << index << endl;
    size_t write_end_position = (index+2)*(object_size);

    bool expanding = false;
    if(write_end_position > storage_area.size()) {
      expanding = true;
    }
 //   if(expanding) cout << "Storage area osize: " << storage_area.size() << endl;
    for(;write_end_position > storage_area.size();) {
      storage_area.push_back(0);
    }
 //   cout << "write_end_position: " << write_end_position << endl;
//    if(expanding) cout << "Storage area nsize: " << storage_area.size() << endl;

    //cout << "object size is: " << object_size << endl;
    for(size_t n=0;n<object_size;n++) {
      //cout << "write at: " << (index*object_size)+n << " , ";
      //char c = ((const char *)(&o))[n];
      //cout << "value   : " << static_cast<int>(c) << endl;
      storage_area[(index*object_size)+n] = ((const char *)(&o))[n];
    }
  }

  void clear() {
    current_max = 0;
    storage_area.clear();
    initialise(0);
  }

  object_type get(size_t index) {
    //cout << "object store get: " << index << endl;
    object_type o;

//    cout << "object size is: " << object_size << endl;
    for(size_t n=0;n<object_size;n++) {
      //cout << "read at: " << (index*object_size)+n << " , ";
      //char c = storage_area[(index*object_size)+n];
      //cout << "value  : " << static_cast<int>(c) << endl;
      ((char *)(&o))[n] = storage_area[(index*object_size)+n];
    }

    return o;
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

// template<class object_type> ObjectStore<object_type>* ObjectStore<object_type>::m_instance=0;

#endif
