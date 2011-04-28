#ifndef CHILDSTORE
#define CHILDSTORE

#include <vector>
#include <stdint.h>

using namespace std;

class SymbolPair {

  public:

  SymbolPair(uint8_t s,int32_t i) : symbol(s),index(i) {
  }

  uint8_t symbol;
  int32_t index;
};

class ChildStore {

public:
  ChildStore() {
  }

  void set(char symbol,int index) {

    // attempt locate
    for(size_t n=0;n<m_symbols.size();n++) {
      if(m_symbols[n].symbol == symbol) {
        m_symbols[n].index = index; 
        if(index == -1) {m_symbols.erase(m_symbols.begin()+n,m_symbols.begin()+n);}
        return;
      }
    }

    m_symbols.push_back(SymbolPair(symbol,index));
    return;
  }

  int get(char symbol) {
    for(size_t n=0;n<m_symbols.size();n++) {
      if(m_symbols[n].symbol == symbol) {
        return m_symbols[n].index;
      }
    }
    return -1;
  }

  int size() {
    return m_symbols.size();
  }

  void clear() {
    m_symbols.clear();
  }

  bool operator==(ChildStore &other) {
    return equal(other);
  }

  bool equal(ChildStore &other,bool dump=false) {
    for(size_t n=0;n<m_symbols.size();n++) {
      if(other.get(m_symbols[n].symbol) != m_symbols[n].index) { cout << "child of symbol " << static_cast<int>(m_symbols[n].symbol) << " does not match, my idx is: " << m_symbols[n].index << " other is: " << other.get(m_symbols[n].symbol) << endl; return false; }
    }

    return true;
  }

  int32_t get_first() {
    if(m_symbols.size()==0) return -1;// possibly not required SPEED
    return m_symbols[0].index;
  }

  int32_t get_last() {
    if(m_symbols.size()==0) return -1;// possibly not required SPEED
    return m_symbols[m_symbols.size()-1].index;
  }

  int32_t next_child(int32_t idx) {
    bool next=false;
    for(size_t n=0;n<m_symbols.size();n++) {
      if(next==true) {
        return m_symbols[n].index;
      }
      if(m_symbols[n].index == idx) { next=true; }
    }
    return -1;
  }

  bool is_child(int32_t idx) {

    for(size_t n=0;n<m_symbols.size();n++) if(m_symbols[n].index == idx) return true;
    return false;
  }

  bool is_leaf() {
    if(m_symbols.size() == 0) return true; else return false;
  }

  const vector<SymbolPair> &get_symbols() {
    return m_symbols;
  }

  void set_symbols(const vector<SymbolPair> &s) {
    m_symbols = s;
  }

  void dump() {
  }

private:
  vector<SymbolPair> m_symbols;
};

#endif
