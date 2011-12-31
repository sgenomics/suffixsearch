#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "UnitTest.h"
#include "tialloc.h"

using namespace std;

void test_tialloc_random(UnitTest &utf) {


  vector<void *> all_allocations;
  vector<int   > all_allocations_size;

  for(size_t n=0;n<1000;n++) {
    int allocation_size = rand()%1000;
    
    unsigned char *allocation = (unsigned char *) tialloc::instance()->alloc(allocation_size);

    // fill allocated memory
    for(size_t i=0;i<allocation_size;i++) {
      allocation[i] = i%256;
    }

    all_allocations.push_back(allocation);
    all_allocations_size.push_back(allocation_size);
  }

  // check allocations
  for(size_t n=0;n<all_allocations.size();n++) {
    for(size_t i=0;i<all_allocations_size[n];i++) {
      unsigned char v = ((unsigned char *) all_allocations[n])[i];
      utf.test_equality((int)v,(int) i%256);
    }
  }

}



int test_tialloc(UnitTest &utf) {

  utf.begin_test_set("tialloc tests");

  test_tialloc_random(utf);

  utf.end_test_set();
}
