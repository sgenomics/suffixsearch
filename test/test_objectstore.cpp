#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "ObjectStore.h"
#include "UnitTest.h"

using namespace std;

int test_objectstore(UnitTest &utf) {

  utf.begin_test_set("ObjectStore tests");

  ObjectStore<int> store(5);

  for(size_t n=0;n<100000;n++) {
    store.push_back(n);
    utf.test_equality(store.get(n),static_cast<int>(n));
  }

  for(size_t n=0;n<100000;n++) {
    store.set(n,n*2);
    utf.test_equality(store.get(n),static_cast<int>(n*2));
  }

  for(size_t n=0;n<100000;n++) {
    size_t m = rand()%100000;
    utf.test_equality(store.get(m),static_cast<int>(m*2));
  }

  for(size_t n=0;n<100000;n++) {
    size_t m = rand()%100000;
    store.set(m,12);
    utf.test_equality(store.get(m),12);
  }

  utf.end_test_set();
}
