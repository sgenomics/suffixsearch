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

//    cout << "adding child: " << static_cast<int>(symbol) << "," << index << endl;
    if(get(symbol) == -1) {
      m_symbols.push_back(SymbolPair(symbol,index));
    } else
    for(size_t n=0;n<m_symbols.size();n++) {
      if(m_symbols[n].symbol == symbol) {
   ////     if(index != -1) m_symbols[n].index = index;
        m_symbols[n].index = index;
        if(index == -1) {m_symbols.erase(m_symbols.begin()+n,m_symbols.begin()+n); break;}
      }
    }

    // dump
 /*   for(size_t n=0;n<m_symbols.size();n++) {
      cout << "child list: " << static_cast<int>(m_symbols[n].symbol) << "," << m_symbols[n].index << endl;
    }
*/
  }

  int get(char symbol) {
//    cout << "ChildStore, getting child: " << static_cast<int>(symbol) << endl;
//    cout << "m_symbols.size(): " << m_symbols.size() << endl;
    for(size_t n=0;n<m_symbols.size();n++) {
      if(m_symbols[n].symbol == symbol) {
//        cout << "found at position: " << n << endl;
//        cout << "returning: " << m_symbols[n].index << endl;
        return m_symbols[n].index;
      }

//      cout << "not found at position: " << n << " " << static_cast<int>(m_symbols[n].symbol)  << "," << m_symbols[n].index << endl;
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

  vector<SymbolPair> m_symbols;
};

#endif
