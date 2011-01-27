#ifndef SUFFIX4_OBJECTSTOREDISK
#define SUFFIX4_OBJECTSTOREDISK

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <fstream>
#include "stringify.h"
#include <stdlib.h>

using namespace std;

template<class object_type>
class ObjectStoreDisk {

private:

  string storage_filename;
  fstream *storage_file;

public:
  size_t current_max;
  size_t object_size;
  typedef object_type value_type;

  ObjectStoreDisk(int suggested_initial_size=0) : current_max(0) {
    initialise();

    object_size = sizeof(object_type);
  }

  ~ObjectStoreDisk() {
    delete storage_file;
  }

  void initialise() {

    // create a new file in which to store data

    storage_filename = string("./temp_") + stringify(rand());
    cout << "creating storage_area file: " << storage_filename << endl;

    storage_file = new fstream(storage_filename.c_str(), ios_base::in | ios_base::out | ios_base::binary | ios_base::trunc);
    bool r = (*storage_file).fail();
    if(r == true) cout << "YOU FAILURE" << endl;
    (*storage_file) << "nothing";
    object_size = sizeof(object_type);
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
    }

    const char *base_pointer = reinterpret_cast<const char *> (&o);
    storage_file->seekp(index*object_size);
    for(size_t n=0;n<object_size;n++) {
      uint8_t c = ((const char *) (base_pointer+n))[0];
      storage_file->put(c);
    }
  }

  void clear() {
    current_max = 0;

    // TODO: ADD DELETE FILE CODE
    initialise(0);
  }

  object_type get(size_t index) {

    object_type o;

    char *base_pointer = reinterpret_cast<char *>(&o);
    size_t base_read = index*object_size;
    storage_file->seekg(base_read);

    char *end_pointer  = base_pointer+object_size;
    for(;base_pointer<end_pointer;base_pointer++) {
      *(base_pointer) = storage_file->get(); 
    }

    return o;
  }

  size_t push_back(const object_type &o) {
    set(current_max,o);
    current_max++;
    return current_max-1;
  }

  size_t get_max() {
    return current_max;
  }

  void set_size(size_t new_size) {
    current_max = new_size;
  }

  size_t size() {
    return get_max();
  }

};

#endif
