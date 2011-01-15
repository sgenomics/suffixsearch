#ifndef SUFFIX4_OBJECTSTOREDISK
#define SUFFIX4_OBJECTSTOREDISK

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <fstream>
#include "stringify.h"

using namespace std;

template<class object_type>
class ObjectStoreDisk {

private:

  size_t object_size;

  string storage_filename;
  fstream *storage_file;

public:
  size_t current_max;
  typedef object_type value_type;

  ObjectStoreDisk() : current_max(0) {
    initialise();
  }

  void initialise() {

    // create a new file in which to store data

    storage_filename = string("temp_") + stringify(rand());
    cout << "creating storage_area file: " << storage_filename << endl;

    storage_file = new fstream(storage_filename.c_str());

  }

  size_t get_file_size() {
    storage_file->seekg(0,ios_base::end);
    size_t pos = storage_file->tellg();
    return pos;
  }

  void set(size_t index,const object_type &o) {

    size_t write_end_position = (index+2)*(object_size);

    size_t current_file_size = get_file_size();

    bool expanding = false;
    if(write_end_position > current_file_size) {
      expanding = true;
    }

   // increase filesize, until we are able to store data at this location.
   for(;write_end_position > current_file_size;) {
     storage_file->seekg(0,ios_base::end);
     storage_file->put(0);
     current_file_size = get_file_size();
     current_max = current_file_size;
   }

    const char *base_pointer = reinterpret_cast<const char *> (&o);
    storage_file->seekp(index*object_size);
    for(size_t n=0;n<object_size;n++) {

      uint8_t c = ((const char *) (base_pointer+n))[0]; //((const char *)(&o))[n];
      storage_file->put(c);
    }
  }

  void clear() {
    current_max = 0;
    //storage_area.clear();

    // TODO: ADD DELETE FILE CODE
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

    storage_file->seekg(base_read);
    for(size_t n=0;n<object_size;n++) {
      *(base_pointer+n) = storage_file->get(); //storage_area[base_read+n];
    }

    object_type oo = *(reinterpret_cast<object_type *>(o));

    delete [] o;

    return oo;
  }

  size_t push_back(const object_type &o) {
    set(current_max,o);
    current_max++;
    return current_max-1;
  }

  size_t get_max() {
    return current_max;
  }

  size_t size() {
    return get_max();
  }

};

#endif
