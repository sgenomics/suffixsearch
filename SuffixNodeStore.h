#ifndef SUFFIXNODESTORE
#define SUFFIXNODESTORE

#include <vector>

using namespace std;

#include "SuffixNode.h"

class SuffixNodeStore {

public:
  SuffixNodeStore() {
  }

  size_t push_back() {
    m_store.push_back(SuffixNode(0,0,0));
    return m_store.size()-1;
  }

  size_t push_back(SuffixNode &s) {
    m_store.push_back(s);
    return m_store.size()-1;
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
