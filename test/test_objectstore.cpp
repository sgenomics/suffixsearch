#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "ObjectStore.h"
#include "SuffixNodeStore.h"
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

  ObjectStore<NormalSuffixNodeContainer> store2(5);

  for(int32_t n=0;n<100000;n++) {

    NormalSuffixNodeContainer no;
    no.parent            = n;
    no.label_start       = n;
    no.suffix_link       = n;
    no.next_left_leaf    = n;
    no.next_right_leaf   = n;
    no.depth             = n;
    no.childlist_idx     = n;
    int id = store2.push_back(no);

    utf.test_equality(store2.get(id).parent         ,n);
    utf.test_equality(store2.get(id).label_start    ,n);
    utf.test_equality(store2.get(id).suffix_link    ,n);
    utf.test_equality(store2.get(id).next_left_leaf ,n);
    utf.test_equality(store2.get(id).next_right_leaf,n);
    utf.test_equality(store2.get(id).depth          ,n);
    utf.test_equality(store2.get(id).childlist_idx  ,n);
  }

  
  ObjectStore<ChildList<7> > store3(5);
  for(int32_t n=0;n<100000;n++) {
    ChildList<7> c;

    vector<SymbolPair> vs;

    vs.push_back(SymbolPair(rand(),rand()));
    vs.push_back(SymbolPair(rand(),rand()));
    vs.push_back(SymbolPair(rand(),rand()));
    vs.push_back(SymbolPair(rand(),rand()));
    vs.push_back(SymbolPair(rand(),rand()));
    vs.push_back(SymbolPair(rand(),rand()));
    vs.push_back(SymbolPair(rand(),rand()));

    c.set_children(vs);

    int id = store3.push_back(c);

    ChildList<7> nc = store3.get(id);

    utf.test_truth(c.equal(nc));
  }
  


  utf.end_test_set();
}
