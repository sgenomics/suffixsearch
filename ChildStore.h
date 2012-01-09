#ifndef CHILDSTORE
#define CHILDSTORE

#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include "tialloc.h"

using namespace std;

class SymbolPair {

  public:

  SymbolPair(uint8_t s,int32_t i) : symbol(s),index(i) {
  }

  uint8_t symbol;
  int32_t index;
} __attribute__((__packed__));

class ChildStore {

public:
  ChildStore() {
    m_symbols = 0;
    m_symbols_size =0;
  }

  ~ChildStore() {
 //   if(m_symbols!=0) free(m_symbols);
  }

  void set(char symbol,int index) {

    // attempt locate
    for(size_t n=0;n<m_symbols_size;n++) {
      if(m_symbols[n].symbol == symbol) {
        m_symbols[n].index = index; 
        if(index == -1) {m_symbols_erase(n);}
        return;
      }
    }

    m_symbols_push_back(SymbolPair(symbol,index));
    return;
  }

  void m_symbols_erase(size_t i) {
    for(size_t n=i;n<(m_symbols_size-1);n++) {
      m_symbols[n] = m_symbols[n+1];
    }
    m_symbols_size--;
  }

  void m_symbols_push_back(SymbolPair s) {
    if(m_symbols_size == 0) {
      m_symbols = (SymbolPair *) tialloc::instance()->alloc(sizeof(SymbolPair));
      m_symbols_size = 1;
    } else {
      m_symbols = (SymbolPair *) tialloc::instance()->realloc(m_symbols,(m_symbols_size+1)*sizeof(SymbolPair));
      m_symbols_size += 1;
    }

    m_symbols[m_symbols_size-1] = s;
  }

  int get(char symbol) {
    for(size_t n=0;n<m_symbols_size;n++) {
      if(m_symbols[n].symbol == symbol) {
        return m_symbols[n].index;
      }
    }
    return -1;
  }

  int size() {
    return m_symbols_size;
  }

  void clear() {
    if(m_symbols_size != 0) delete m_symbols;
    m_symbols_size = 0;
  }

  bool operator==(ChildStore &other) {
    return equal(other);
  }

  bool equal(ChildStore &other,bool dump=false) {
    for(size_t n=0;n<m_symbols_size;n++) {
      if(other.get(m_symbols[n].symbol) != m_symbols[n].index) { cout << "child of symbol " << static_cast<int>(m_symbols[n].symbol) << " does not match, my idx is: " << m_symbols[n].index << " other is: " << other.get(m_symbols[n].symbol) << endl; return false; }
    }

    return true;
  }

  int32_t get_first() {
    if(m_symbols_size==0) return -1;// possibly not required SPEED
    return m_symbols[0].index;
  }

  int32_t get_last() {
    if(m_symbols_size==0) return -1;// possibly not required SPEED
    return m_symbols[m_symbols_size-1].index;
  }

  int32_t next_child(int32_t idx) {
    bool next=false;
    for(size_t n=0;n<m_symbols_size;n++) {
      if(next==true) {
        return m_symbols[n].index;
      }
      if(m_symbols[n].index == idx) { next=true; }
    }
    return -1;
  }

  bool is_child(int32_t idx) {

    for(size_t n=0;n<m_symbols_size;n++) if(m_symbols[n].index == idx) return true;
    return false;
  }

  bool is_leaf() {
    if(m_symbols_size == 0) return true; else return false;
  }

  const vector<SymbolPair> get_symbols() {
    vector<SymbolPair> symbols;

    for(size_t n=0;n<m_symbols_size;n++) symbols.push_back(symbols[n]);

    return symbols;
  }

  void set_symbols(const vector<SymbolPair> &s) {
    clear();

    for(size_t n=0;n<s.size();n++) m_symbols_push_back(s[n]);

 //   m_symbols = s;
  }

  void dump() {
    for(size_t n=0;n<m_symbols_size;n++) {
        cout << (int) n << "," << (int) m_symbols[n].index << " ";
    }
    cout << endl;
  }

private:
//  vector<SymbolPair> m_symbols;
  SymbolPair *m_symbols;
  char        m_symbols_size;
} __attribute__((__packed__));

#endif
