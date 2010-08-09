#ifndef SUFFIXNODESTORE
#define SUFFIXNODESTORE

#include <vector>

using namespace std;

#include "SuffixNode.h"

class SuffixNodeStore {

public:
  SuffixNodeStore() {
  }

  void push_back(SuffixNode &s) {
    m_store.push_back(s);
  }

  SuffixNode get(int idx) {
    return m_store[idx];
  }

  void set(int idx, SuffixNode &s) {
    m_store[idx] = s;
  }

  int size() {
    return m_store.size();
  }

  vector<SuffixNode> m_store;
};

#endif
