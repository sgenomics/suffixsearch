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

using namespace std;

#include "SuffixNode.h"

#include <stdint.h>

class SuffixNodeStoreMemVec {

public:
  SuffixNodeStoreMemVec() {
    compact_enabled = true;
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

    storage_area.push_back(s);

    return storage_area.size()-1;
  }

  SuffixNode &get(int idx) {
    return storage_area[idx];

  }

  void set(int idx, SuffixNode &s) {
  }

  int size() {
    return storage_area.size();
  }

  int next_idx(int i) {
    return i+1;
  }

  int last_idx() {
    return storage_area.size();
  }

  void stats() {
    cout << "Storage area size: " << storage_area.size() << endl;
  }

  void force_compact() {
  }

  void compact() {
  }

  vector<SuffixNode> storage_area;

  bool compact_enabled;
};


#endif
