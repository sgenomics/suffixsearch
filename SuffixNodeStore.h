#ifndef SUFFIXNODESTORE
#define SUFFIXNODESTORE

#include <vector>

using namespace std;

#include "SuffixNode.h"

#include <stdint.h>

class EndSuffixNode {

  public:

    int32_t parent;
    int32_t label_start;
    int32_t suffix_link;
    int32_t next_left_leaf;
    int32_t next_right_leaf;
    int32_t depth;

    EndSuffixNode(SuffixNode &s) {
      parent          = s.parent;
      label_start     = s.label_start;
      suffix_link     = s.suffix_link;
      next_left_leaf  = s.next_left_leaf;
      next_right_leaf = s.next_right_leaf;
      depth           = s.depth;
    }


    SuffixNode get_suffixnode() {
      SuffixNode s(0,0,0);
      s.parent          = parent;
      s.label_start     = label_start;
      s.suffix_link     = suffix_link;
      s.next_left_leaf  = next_left_leaf;
      s.next_right_leaf = next_right_leaf;
      s.depth           = depth;

      return s;
    }
};

class SuffixNodeStore {

public:
  SuffixNodeStore() {
  }

  size_t push_back() {
    m_store1.push_back(SuffixNode(0,0,0));
    return m_store1.size()-1;
  }

  size_t push_back(SuffixNode &s) {

    if((s.label_end == -1) && (s.label_start != -1)) {
      EndSuffixNode s2(s);


      m_store2.push_back(s2);
      return m_store2.size()-1 + 0x01000000;

    } else {
      m_store1.push_back(s);
      return m_store1.size()-1;
    }
  }

  SuffixNode get(int idx) {
    int id = get_store_id(idx);
    if(id == 0) return m_store1[idx];
    if(id == 1) return m_store2[idx-0x01000000].get_suffixnode();
  }

  void set(int idx, SuffixNode &s) {
    int id = get_store_id(idx);
    if(id == 0) m_store1[idx] = s;
    if(id == 1) m_store2[idx-0x01000000] = EndSuffixNode(s);
  }

  int size() {
    return m_store1.size()+m_store2.size();
  }

  int get_store_id(int i) {
    return (0xFF000000 & i) >> 24;
  }

  int next_idx(int i) {
    int id = get_store_id(i);

    if(id == 0) {
      if(i+1 < m_store1.size()) return i+1;
      return 0x01000000;
    } else return i++;
  }

  bool last_idx() {
    return 0x01000000+m_store2.size();
  }

  vector<SuffixNode>    m_store1;
  vector<EndSuffixNode> m_store2;
};

#endif
