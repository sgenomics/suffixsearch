#ifndef CHILDSTORE
#define CHILDSTORE

#include <vector>
#include <stdint.h>
#include "global_defs.h"

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
    clear();
  }

  void set(char symbol,int index) {
    if(m_symbols[0] < 0) m_symbols[0] = -1;
    m_symbols[symbol] = index;
  }

  int get(char symbol) {

    if(m_symbols[symbol] < 0) return -1;
    return m_symbols[symbol];
  }

  int size() {
    int size=0;
    for(size_t n=0;n<symbol_size;n++) {
      if(m_symbols[n] >= 0) size++;
    }
    return size;
  }

  void clear() {
    for(size_t n=0;n<symbol_size;n++) { m_symbols[n] = -2; }
  }

  bool is_leaf() { if(m_symbols[0] == -2) return true; else return false; } //return m_is_leaf; }

  bool operator==(ChildStore &other) {
    return equal(other);
  }

  bool equal(ChildStore &other,bool dump=false) {
    for(size_t n=0;n<symbol_size;n++) { 
      if(get(n) != other.get(n)) return false;
    }

    return true;
  }

  vector<SymbolPair> get_symbols() {

    vector<SymbolPair> symbols_tmp;

    for(size_t n=0;n<symbol_size;n++) {
      if(m_symbols[n] >= 0) symbols_tmp.push_back(SymbolPair(n,m_symbols[n]));
    }

    return symbols_tmp;
  }

  void set_symbols(const vector<SymbolPair> &s) {
    clear();
    for(size_t n=0;n<s.size();n++) {
      m_symbols[s[n].symbol] = s[n].index;
    }
    if(m_symbols[0] < 0) m_symbols[0] = -1;
  }

  int32_t get_first() {
    for(size_t n=0;n<symbol_size;n++) {
      if(m_symbols[n] >= 0) return m_symbols[n];
    }
    return -1;
  }
  
  int32_t get_last() {
    for(int32_t n=symbol_size-1;n>=0;n--) {
      if(m_symbols[n] >= 0) return m_symbols[n];
    }
    return -1;
  }

  int32_t next_child(int32_t idx) {
    bool next=false;
    for(size_t n=0;n<symbol_size;n++) {
      if(next==true) {
        if(m_symbols[n] >= 0) { return m_symbols[n];}
      }
      if(m_symbols[n] == idx) { next=true; }
    }
    return -1;
  }

  bool is_child(int32_t idx) {
    for(size_t n=0;n<symbol_size;n++) {
      if(m_symbols[n] == idx) return true;
    }
    return false;
  }

  void dump() {
    for(size_t n=0;n<symbol_size;n++) {
      cout << n << " : " << m_symbols[n] << endl;
    }
  }

private:
  int32_t m_symbols[symbol_size];
};

#endif
