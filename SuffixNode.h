#ifndef SUFFIXNODE
#define SUFFIXNODE

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <stdlib.h>
#include "ChildStore.h"
#include <stdint.h>


using namespace std;

#define symbol_size  41
#define final_symbol 39

class SuffixNode {

public:

  SuffixNode() {
    m_children = 0;
    label_end       = -1;
    next_left_leaf  = -1;
    next_right_leaf = -1;
  }

  ~SuffixNode() {
  }

  SuffixNode(int parent_in,int label_start_in,int depth_in) : parent(parent_in),label_start(label_start_in), depth(depth_in) {

    suffix_link = 0;
    m_children = 0;

    label_end       = -1;
    next_left_leaf  = -1;
    next_right_leaf = -1;
  }

  bool is_leaf() {
    if(m_children == 0) return true;
    return m_children->is_leaf();
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
    if(m_children != 0) {m_children->free(); delete m_children;}
    m_children = 0;
  }

  void copy_children(SuffixNode &other) {

    if(other.m_children == 0) {clear_children(); m_children = 0; return;}

    if(m_children == 0) m_children = new ChildStore();

    (*m_children) = (*(other.m_children));
  }

  void replace_children(int64_t old_id,int64_t new_id) {
    for(int n=0;n<symbol_size;n++) {
      if(get_child(n) == old_id) set_child(n,new_id);
    }
  }

  int first_child() {

    for(int64_t n=0;n<symbol_size;n++) {
      if(get_child(n) != -1) return n;
    }

    return -1;
  }

  int find_child(int c) {

    for(int64_t n=0;n<symbol_size;n++) {
      if(get_child(n) == c) return n;
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
    if(m_children != 0) return m_children->size(); else return 0;
//    return m_children.size();
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

  int get_child(int n) {
//    cout << "SuffixNode getting child: " << n << endl;
    if(is_leaf()) return -1;

    return m_children->get(n);
  }

  void set_child(int n,int m) {
    if(m_children == 0) m_children = new ChildStore();
    m_children->set(n,m);
  }

  bool operator==(SuffixNode &other) {

    return equal(other); 
  }

  bool is_child(int32_t idx) {
    if(m_children == 0) return false;
    return m_children->is_child(idx);
  }

  int32_t next_child(int32_t idx) {
    if(m_children == 0) return -1;
    return m_children->next_child(idx);
  }

  int32_t get_first_child() {
    if(m_children ==0) return -1;
    return m_children->get_first();
  }

  int32_t get_last_child() {
    if(m_children ==0) return -1;
    return m_children->get_last();
  }

  bool equal(SuffixNode &other,bool dump=false) {
    if(parent          != other.parent     )    { if(dump)  cout << "parent match failure" << endl;          return false; }
    if(label_start     != other.label_start)    { if(dump)  cout << "label_start match failure" << endl;     return false; }
    if(label_end       != other.label_end  )    { if(dump)  cout << "label_end match failure mine: " << label_end << " other: " << other.label_end << endl; return false; }
 if(m_children != 0)   if((m_children->equal(*(other.m_children),dump) == false))    { if(dump)  cout << "children match failure" << endl; return false; }
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
    cout << "children ptr   : " << m_children << endl;
    if(m_children != 0) m_children->dump();
  }

  int32_t parent;
  int32_t label_start;
  int32_t label_end  ;
  ChildStore *m_children;
  int32_t suffix_link;
  int32_t next_left_leaf;
  int32_t next_right_leaf;
  int32_t depth;

  static int32_t end_marker;
  static int32_t end_marker_value;
  static int32_t root;
} __attribute__((__packed__));

#endif
