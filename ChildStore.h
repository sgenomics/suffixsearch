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
    for(size_t n=0;n<alphabet_size;n++) {
      if(m_symbols[n] != -1) size++;
    }
    return size;
  //  return m_symbols.size();
  }

  void clear() {
    for(size_t n=0;n<alphabet_size;n++) { m_symbols[n] = -1; }

    //m_symbols.clear();
  }

  bool operator==(ChildStore &other) {
    return equal(other);
  }

  bool equal(ChildStore &other,bool dump=false) {
/*    for(size_t n=0;n<m_symbols.size();n++) {
      if(other.get(m_symbols[n].symbol) != m_symbols[n].index) { cout << "child of symbol " << static_cast<int>(m_symbols[n].symbol) << " does not match, my idx is: " << m_symbols[n].index << " other is: " << other.get(m_symbols[n].symbol) << endl; return false; }
    }
*/
    for(size_t n=0;n<alphabet_size;n++) {
      if(get(n) != other.get(n)) return false;
    }

    return true;
  }

  vector<SymbolPair> &get_symbols() {

    symbols_tmp.clear();

    for(size_t n=0;n<alphabet_size;n++) {
      if(m_symbols[n] != -1) symbols_tmp.push_back(SymbolPair(n,m_symbols[n]));
    }

    return symbols_tmp;
  }

  void set_symbols(vector<SymbolPair> s) {
    clear();
    for(size_t n=0;n<s.size();n++) {
      m_symbols[s[n].symbol] = s[n].index;
    }
    //m_symbols = s;
  }

private:

    vector<SymbolPair> symbols_tmp; // REALLY MESSY!!! REMOVE THIS REQUIREMENT
  size_t m_symbols[alphabet_size];
//  vector<SymbolPair> m_symbols;

};

#endif
