#include "tialloc.h"

int main() {

  cout << "size of small_block is: " << sizeof(small_block) << endl;

  tialloc::instance()->dump();

  cout << "**************************** Initialisation dump complete" << endl;

  char *i0 = (char *) tialloc::instance()->alloc(1);

  cout << "alloc pointer i0 is: " << ((void *)i0) << endl;
  *i0 = 0xFE;
  
  cout << "**************************** dump after first alloc follows" << endl;
  tialloc::instance()->dump();
  cout << "**************************** dump after first alloc complete" << endl;


  char *i1 = (char *) tialloc::instance()->alloc(1);
  char *i2 = (char *) tialloc::instance()->alloc(1);
  char *i3 = (char *) tialloc::instance()->alloc(1);

  *i1 = 0xFD;
  *i2 = 0xFE;
  *i3 = 0xFD;
  
  cout << "**************************** dump after all allocs follows" << endl;
  tialloc::instance()->dump();
  cout << "**************************** dump after all allocs complete" << endl;

  tialloc::instance()->free(i2);
  tialloc::instance()->free(i3);
  tialloc::instance()->free(i1);
  tialloc::instance()->free(i0);

  cout << "***************************** Final dump" << endl;
  tialloc::instance()->dump();

  vector<uint8_t  > values;
  vector<uint8_t *> memorylocations;

  // test size 1 allocations:
  // fill
  for(int n=0;n<10000;n++) {
    
    uint8_t rand_val = rand()%256;

    uint8_t *i = (uint8_t *) tialloc::instance()->alloc(1);
    *i = rand_val;

    values.push_back(rand_val);
    memorylocations.push_back(i);

    // self validate
    uint8_t value = *(memorylocations[memorylocations.size()-1]);
    if(value != values[values.size()-1]) {
      cout << "self storage validation error, " << (int) value << " != " << (int) values[values.size()-1] << "    " << n << endl;
      int *i =0; *i = 1;
    }
    value = *(memorylocations[memorylocations.size()-1]);
  }

  // verify
  for(int n=0;n<values.size();n++) {
    uint8_t value = *(memorylocations[n]);
    if(n%1000 == 0) cout << "validated: " << n << endl;
    if(value != values[n]) cout << "storage validation error, " << (int) value << " != " << (int) values[n] << "    " << n << endl;
  }

  // free
  for(int n=0;n<memorylocations.size();n++) {
    tialloc::instance()->free(memorylocations[n]);
  }

cout << "DIFFERENT SIZES" << endl;


  // test random sized allocations
  vector<uint32_t  > values4;
  vector<uint32_t *> memorylocations4;
  // fill
  for(int n=0;n<10000;n++) {
    
    uint32_t rand_val = rand();

    uint32_t *i = (uint32_t *) tialloc::instance()->alloc(4);
    *i = rand_val;

    values4.push_back(rand_val);
    memorylocations4.push_back(i);

    // self validate
    uint32_t value = *(memorylocations4[memorylocations4.size()-1]);
    if(value != values4[values4.size()-1]) {
      cout << "self storage validation error, " << (int) value << " != " << (int) values4[values4.size()-1] << "    " << n << endl;
      int *i =0; *i = 1;
    }
    value = *(memorylocations4[memorylocations4.size()-1]);
  }

  // verify
  for(int n=0;n<values4.size();n++) {
    uint32_t value = *(memorylocations4[n]);
    if(n%1000 == 0) cout << "validated: " << n << endl;
    if(value != values4[n]) cout << "storage validation error, " << (int) value << " != " << (int) values4[n] << "    " << n << endl;
  }

  // free
  for(int n=0;n<memorylocations4.size();n++) {
    tialloc::instance()->free(memorylocations4[n]);
  }

  char *t = (char *) tialloc::instance()->alloc(10);

  for(size_t n=0;n<10;n++) t[n] = n;

  t =(char *)  tialloc::instance()->realloc(t,250);

  for(size_t n=0;n<10;n++) {
    if(t[n] != n) cout << "TEST FAILED" << endl;
             else cout << "TEST PASSED" << endl;
  }


}
