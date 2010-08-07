#ifndef SUFFIXNODESTORE
#define SUFFIXNODESTORE

#include <vector>

using namespace std;

class SuffixNode;

class SuffixNodeStore {

public:
  SuffixNodeStore() {
  }

  void push_back(SuffixNode &s) {
    m_store.push_back(s);
  }

  SuffixNode &operator[](size_t i) {
    return m_store[i];
  }

  int size() {
    return m_store.size();
  }

  vector<SuffixNode> m_store;
};

#endif
