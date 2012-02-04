#ifndef SUFFIXNODE
#define SUFFIXNODE

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <stdlib.h>
#include <stdint.h>
#include "tialloc.h"

using namespace std;

#define symbol_size  41
#define final_symbol 39

class SymbolPair {
  public:

  SymbolPair(uint8_t s,int32_t i) : symbol(s),index(i) {
  }

  uint8_t symbol;
  int32_t index;
} __attribute__((__packed__));

class SuffixNode {

public:

  SuffixNode() {
    label_start     = -1;
    label_end       = -1;
    next_left_leaf  = -1;
    next_right_leaf = -1;
    depth           = -1;
    parent          = -1;
    m_symbols = 0;
    m_symbols_size = 0;
  }

  ~SuffixNode() {
  }

  SuffixNode(int parent_in,int label_start_in,int depth_in) : parent(parent_in),label_start(label_start_in), depth(depth_in) {

    suffix_link = 0;
    m_symbols = 0;
    m_symbols_size = 0;

    label_end       = -1;
    next_left_leaf  = -1;
    next_right_leaf = -1;
  }

  bool is_leaf() {
    if(m_symbols_size == 0) return true;
    return false;
  }

  int get_label_length() {
    if(label_start == -1) return 0;

    if(label_end == end_marker) {
      return end_marker_value-label_end; // THIS IS WRONG BUT SOMETHING NEEDS IT ARGH!
    }

    return label_end-label_start;
  }

  int get_label_length_r() {
    if(label_start == -1) return 0;

    if(label_end == end_marker) {
      return end_marker_value-label_start; 
    }

    return label_end-label_start;
  }

  void clear_children() {
    m_symbols_size = 0;
    if(m_symbols != 0) tialloc::instance()->free(m_symbols);
    m_symbols = 0;
  }

  void copy_children(SuffixNode &other) {

    clear_children();
    if(other.m_symbols_size == 0) return;

    m_symbols = (SymbolPair *) tialloc::instance()->alloc(sizeof(other.m_symbols_size*sizeof(SymbolPair)));
    m_symbols_size = other.m_symbols_size;

    for(size_t n=0;n<m_symbols_size;n++) {
      m_symbols[n].symbol = other.m_symbols[n].symbol;
      m_symbols[n].index  = other.m_symbols[n].index;
    }
  }

/*
  // TODO: check if this needs to be able to cope with new_id == -1
  void replace_children(int32_t old_id,int32_t new_id) {
    for(int n=0;n<m_symbols_size;n++) {
      if(m_symbols[n].index == old_id) {set_child(old_id,new_id); n--;}
    }
  }
*/
/*
  int first_child() {

    if(m_symbols_size == 0) return -1;

    uint8_t min_symbol = 0xFF;
    for(size_t n=0;n<m_symbols_size;n++) {
      if(m_symbols[n].symbol <= min_symbol) min_symbol = m_symbols[n].symbol;
    }

    return min_symbol;
  }
*/
  int find_child(int c) {

    for(int n=0;n<m_symbols_size;n++) {
      if(m_symbols[n].index == c) return n;
    }
    return -1;
  }

  int get_depth() {
    if(label_start == -1) return 0;
    if(label_end   == -1) return depth + (end_marker_value-label_start)+1;
    return depth;
  }

  void set_depth(int depth_in) {
    depth = depth_in;
  }

  int child_count() {
    return m_symbols_size;
  }

  int get_parent() {
    return parent;
  }
  
  int get_label_end() {
    if(label_end == end_marker) {
      return end_marker_value;
    }
    return label_end;
  }

  int get_child(uint8_t symbol) {
    if(m_symbols_size == 0) return -1;

    for(int n=0;n<m_symbols_size;n++) {
      if(m_symbols[n].symbol == symbol) return m_symbols[n].index;
    }
    return -1;
  }

  int child_local_idx(uint8_t symbol) {
    for(size_t n=0;n<m_symbols_size;n++) {
      if(m_symbols[n].symbol == symbol) return n;
    }
    return -1;
  }

  void set_child(uint8_t n,int32_t m) {
    if(m_symbols_size == 0) {
      if(m == -1) return;
      m_symbols = (SymbolPair *) tialloc::instance()->alloc(sizeof(SymbolPair)*2);
      m_symbols_size = 1;
      m_symbols[0].symbol = n;
      m_symbols[0].index  = m;
    }

    int child = child_local_idx(n);
    if(child != -1) {
      if(m != -1) {
        m_symbols[child].index = m;
      } else {
        // index for -1 means erase the entry.
        for(size_t i=child;i<(m_symbols_size-1);i++) {
          m_symbols[i] = m_symbols[i+1];
        }
        m_symbols_size--;

      }
    } else {
        m_symbols = (SymbolPair *) tialloc::instance()->realloc(m_symbols,(m_symbols_size+1)*sizeof(SymbolPair));
        m_symbols_size++;
        m_symbols[m_symbols_size-1].symbol = n;
        m_symbols[m_symbols_size-1].index  = m;
    }
  }

  bool operator==(SuffixNode &other) {
    return equal(other); 
  }

  bool is_child(int32_t idx) {
    for(size_t n=0;n<m_symbols_size;n++) if(m_symbols[n].index == idx) return true;
    return false;
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

  int32_t get_first_child() {
    if(m_symbols_size==0) return -1;
    return m_symbols[0].index;
  }

  int32_t get_last_child() {
    if(m_symbols_size==0) return -1;
    return m_symbols[m_symbols_size-1].index;
  }

  bool equal(SuffixNode &other,bool dump=false) {
    if(parent          != other.parent     )    { if(dump)  cout << "parent match failure" << endl;          return false; }
    if(label_start     != other.label_start)    { if(dump)  cout << "label_start match failure" << endl;     return false; }
    if(label_end       != other.label_end  )    { if(dump)  cout << "label_end match failure mine: " << label_end << " other: " << other.label_end << endl; return false; }
// if(m_children != 0)   if((m_children->equal(*(other.m_children),dump) == false))    { if(dump)  cout << "children match failure" << endl; return false; }

    if(m_symbols_size  != other.m_symbols_size) { if(dump)  cout << "children match failure" << endl; return false; }
    for(size_t n=0;n<m_symbols_size;n++) { if(m_symbols[n].index != other.m_symbols[n].index) {if(dump)  cout << "children match failure" << endl; return false; }    }
    for(size_t n=0;n<m_symbols_size;n++) { if(m_symbols[n].symbol != other.m_symbols[n].symbol) {if(dump)  cout << "children match failure" << endl; return false; }    }

    if(suffix_link     != other.suffix_link)    { if(dump)  cout << "suffix_link match failure" << endl;     return false; }
    if(next_left_leaf  != other.next_left_leaf) { if(dump)  cout << "next_left_leaf match failure" << endl;  return false; }
    if(next_right_leaf != other.next_right_leaf){ if(dump)  cout << "next_right_leaf match failure" << endl; return false; }
    if(depth           != other.depth)          { if(dump)  cout << "depth match failure" << endl;           return false; }

    if(dump) cout << "suffixnodes identical" << endl;
    return true;
  }

  void dump() {
    cout << "SuffixNode" << endl;
    cout << "parent         : " << parent << endl;
    cout << "label_start    : " << label_start << endl;
    cout << "label_end      : " << label_end << endl;
    cout << "suffix_link    : " << suffix_link << endl;
    cout << "next_left_leaf : " << next_left_leaf << endl;
    cout << "next_right_leaf: " << next_right_leaf << endl;
    cout << "depth          : " << depth << endl;
    cout << "children      : ";
    for(size_t n=0;n<m_symbols_size;n++) cout << (int) m_symbols[n].symbol << "," << m_symbols[n].index << " ";
    cout << endl;
  }

  const vector<SymbolPair> get_symbols() {
    vector<SymbolPair> symbols;

    for(size_t n=0;n<m_symbols_size;n++) symbols.push_back(symbols[n]);

    return symbols;
  }

  void set_symbols(const vector<SymbolPair> &s) {
    clear_children();

    for(size_t n=0;n<s.size();n++) set_child(s[n].symbol,s[n].index);
  }


  SymbolPair *m_symbols;
  char        m_symbols_size;

  int32_t parent;
  int32_t label_start;
  int32_t label_end  ;
  //ChildStore *m_children;
  int32_t suffix_link;
  int32_t next_left_leaf;
  int32_t next_right_leaf;
  int32_t depth;

  static int32_t end_marker;
  static int32_t end_marker_value;
  static int32_t root;
} __attribute__((__packed__));

#endif
