#ifndef SUFFIXNODE
#define SUFFIXNODE

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <stdlib.h>
#include "ChildStore.h"

using namespace std;

#define symbol_size  41
#define final_symbol 39

class SuffixNode {

public:

  SuffixNode(int parent_in,int label_start_in,int depth_in) : parent(parent_in),label_start(label_start_in), depth(depth_in),m_children(NULL) {

    clear_children();

    suffix_link = 0;

    label_end       = -1;
    next_left_leaf  = -1;
    next_right_leaf = -1;
  }

  bool isleaf() {
    if(m_children == NULL) return true;
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

    if(m_children != NULL) m_children->clear();
  }

  void copy_children(SuffixNode &other) {
    *m_children = *(other.m_children);
  }

  void replace_children(int64_t old_id,int64_t new_id) {
    for(int n=0;n<symbol_size;n++) {
      if(get_child(n) == old_id) set_child(n,new_id);
    }
  }

  int first_child() {

    if(m_children != NULL)
    for(int64_t n=0;n<symbol_size;n++) {
      if(get_child(n) != -1) return n;
    }

    return -1;
  }

  int find_child(int c) {

    if(m_children != NULL)
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
    int i=0;

    if(m_children != NULL) return m_children->size();
    return i;
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
    if(isleaf()) return -1;

    return m_children->get(n);
  }

  void set_child(int n,int m) {
    if(m_children == NULL) m_children = new ChildStore();
    m_children->set(n,m);
  }

  int parent;
  int label_start;
  int label_end  ;
  ChildStore *m_children;
  int suffix_link;
  int next_left_leaf;
  int next_right_leaf;
  int depth;

  static int end_marker;
  static int end_marker_value;
  static int root;
};

#endif
