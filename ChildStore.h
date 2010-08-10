#ifndef CHILDSTORE
#define CHILDSTORE

#include <vector>

using namespace std;

class SymbolPair {

  public:

  SymbolPair(char s,int i) : symbol(s),index(i) {
  }

  uint8_t symbol;
  int32_t index;
};

class ChildStore {

public:
  ChildStore() {
  }

  void set(char symbol,int index) {
    if(get(symbol) == -1) {
      m_symbols.push_back(SymbolPair(symbol,index));
    } else
    for(size_t n=0;n<m_symbols.size();n++) {
      if(m_symbols[n].symbol == symbol) {
        if(index != -1) m_symbols[n].index = index;
                   else {m_symbols.erase(m_symbols.begin()+n,m_symbols.begin()+n); break;}
      }
    }

  }

  int get(char symbol) {
    for(size_t n=0;n<m_symbols.size();n++) if(m_symbols[n].symbol == symbol) return m_symbols[n].index;
    return -1;
  }

  int size() {
    return m_symbols.size();
  }

  void clear() {
    m_symbols.clear();
  }

  vector<SymbolPair> m_symbols;
};

#endif
