#ifndef SUFFIXNODESTOREMEMVEC
#define SUFFIXNODESTOREMEMVEC

#include "global_defs.h"
#include <vector>
#include <map>
#include <algorithm>
#include "ObjectStoreMem.h"
#include "ObjectStoreMemVec.h"
#include "ObjectStoreDisk.h"
#include "global_defs.h"
#include "tialloc.h"

using namespace std;

#include "SuffixNode.h"

#include <stdint.h>

class SuffixNodeStoreMemVec {

public:
  SuffixNodeStoreMemVec() {
    compact_enabled = true;
    storage_area = (SuffixNode *) malloc(sizeof(SuffixNode)*10000);
    storage_area_size = 0;
    storage_area_real_size = 10000;
  }

  void set_compactmode(bool compact_mode) {
    compact_enabled=compact_mode;
  }

  size_t push_back_norm() {
    SuffixNode s;
    return push_back(s);
  }

  size_t push_back_end() {
    SuffixNode s;
    return push_back(s);
  }

  size_t push_back(SuffixNode &s) {

    //storage_area.push_back(s);

    if(storage_area_real_size > storage_area_size) {
      storage_area[storage_area_size] = s;
      storage_area_size++;
    } else {
      storage_area_real_size += 10000;
      storage_area = (SuffixNode *) realloc(storage_area,storage_area_real_size*(sizeof(SuffixNode)));
      storage_area[storage_area_size] = s;
      storage_area_size++;
    }

    return storage_area_size-1;
  }

  SuffixNode &get(int idx) {
    return storage_area[idx];
  }

  void set(int idx, SuffixNode &s) {
  }

  int size() {
    return storage_area_size;
  }

  int next_idx(int i) {
    return i+1;
  }

  int last_idx() {
    return storage_area_size-1;
  }

  void stats() {
    int leaf_count=0;
    int child_2=0;
    int child_3=0;
    cout << "Storage area size: " << storage_area_size << endl;
    for(size_t n=0;n<storage_area_size;n++) if(storage_area[n].child_count() == 2) child_2++;
    for(size_t n=0;n<storage_area_size;n++) if(storage_area[n].child_count() == 3) child_3++;
    for(size_t n=0;n<storage_area_size;n++) if(storage_area[n].is_leaf()) leaf_count++;
    cout << "Leaf count: " << leaf_count << endl;
    cout << "child2    : " << child_2 << endl;
    cout << "child3    : " << child_3 << endl;
  }

  void force_compact() {
  }

  void compact() {
  }

  SuffixNode *storage_area;
  size_t      storage_area_size;
  size_t      storage_area_real_size;
  // vector<SuffixNode> storage_area;

  bool compact_enabled;
};


#endif
