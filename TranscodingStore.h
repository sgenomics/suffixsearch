#ifndef TRANSCODINGSTORE
#define TRANSCODINGSTORE

#include <vector>

#include <stdio.h>
#include <ctype.h>
using namespace std;


///\brief transcoding module. Converts ascii to limited character set.
class TranscodingStore {

public:
  TranscodingStore() {
  }

  void push_back(char c) {
    m_store.push_back(c);
  }

  char operator[](int i) {
    return t.convert(m_store[i]);
  }

  char get_uncoded(int i) {
    return m_store[i];
  }

  size_t size() {
    return m_store.size();
  }

  Transcode t;
  vector<char> m_store;
};

#endif
