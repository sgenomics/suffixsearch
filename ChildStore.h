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
    m_symbols[symbol] = index;
      m_is_leaf = false;
/*
    if(get(symbol) == -1) {
      m_symbols.push_back(SymbolPair(symbol,index));
    } else
    for(size_t n=0;n<m_symbols.size();n++) {
      if(m_symbols[n].symbol == symbol) {
        m_symbols[n].index = index;
        if(index == -1) {m_symbols.erase(m_symbols.begin()+n,m_symbols.begin()+n); break;}
      }
    }
*/
  }

  int get(char symbol) {
    return m_symbols[symbol];
/*
    for(size_t n=0;n<m_symbols.size();n++) {
      if(m_symbols[n].symbol == symbol) {
        return m_symbols[n].index;
      }
    }
    return -1;
*/
  }

  int size() {
    int size=0;
    for(size_t n=0;n<symbol_size;n++) {
      if(m_symbols[n] != -1) size++;
    }
    return size;
  //  return m_symbols.size();
  }

  void clear() {
    for(size_t n=0;n<=symbol_size;n++) { m_symbols[n] = -1; }

    m_is_leaf = true;
    //m_symbols.clear();
  }

  bool is_leaf() { return m_is_leaf; }

  bool operator==(ChildStore &other) {
    return equal(other);
  }

  bool equal(ChildStore &other,bool dump=false) {
/*    for(size_t n=0;n<m_symbols.size();n++) {
      if(other.get(m_symbols[n].symbol) != m_symbols[n].index) { cout << "child of symbol " << static_cast<int>(m_symbols[n].symbol) << " does not match, my idx is: " << m_symbols[n].index << " other is: " << other.get(m_symbols[n].symbol) << endl; return false; }
    }
*/
    for(size_t n=0;n<symbol_size;n++) { 
      if(get(n) != other.get(n)) return false;
    }

    return true;
  }

  vector<SymbolPair> &get_symbols() {

    symbols_tmp.clear();

    for(size_t n=0;n<symbol_size;n++) {
      if(m_symbols[n] != -1) symbols_tmp.push_back(SymbolPair(n,m_symbols[n]));
    }

    return symbols_tmp;
  }

  void set_symbols(vector<SymbolPair> s) {
    clear();
    for(size_t n=0;n<s.size();n++) {
      m_symbols[s[n].symbol] = s[n].index;
      m_is_leaf = false;
    }
    //m_symbols = s;
  }

  int32_t get_first() {
    for(size_t n=0;n<symbol_size;n++) {
      if(m_symbols[n] != -1) return m_symbols[n];
    }
    return -1;
  }
  
  int32_t get_last() {
    for(size_t n=symbol_size-1;n>=0;n--) {
      if(m_symbols[n] != -1) return m_symbols[n];
    }
    return -1;
  }

  int32_t next_child(int32_t idx) {
    bool next=false;
    cout << "next_child idx is: " << idx << endl;
    for(size_t n=0;n<symbol_size;n++) {
      cout << n << " " << m_symbols[n] << endl;
      if(next==true) {
        if(m_symbols[n] != -1) {cout << "returning: " << m_symbols[n] << endl; return m_symbols[n];}
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

  bool m_is_leaf;
    vector<SymbolPair> symbols_tmp; // REALLY MESSY!!! REMOVE THIS REQUIREMENT
  int m_symbols[symbol_size];
//  vector<SymbolPair> m_symbols;

};

#endif
