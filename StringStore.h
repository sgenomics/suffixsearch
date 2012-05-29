#ifndef STRINGSTORE
#define STRINGSTORE

#include <iostream>
#include <vector>
#include "stringify.h"
#include <string>
#include "global_defs.h"
#include <stdint.h>

using namespace std;

class StringStore {

public:

  string get_string(size_t index) {

    string out_string;

    for(size_t n=index;n<m_string_store.size();n++) {
      if(m_string_store.at(n) != 0) {
        out_string += m_string_store.at(n);
      } else {
        break;
      }
    }

    return out_string;
  }

  size_t add_string(string input_string) {
    size_t idx = m_string_store.size();

    for(size_t n=0;n<input_string.size();n++) {
      m_string_store.push_back(input_string[n]);
    }
    m_string_store.push_back(0);

    return idx;
  }

  vector<uint8_t> m_string_store;
};

#endif
