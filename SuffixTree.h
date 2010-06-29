#ifndef SUFFIXTREE
#define SUFFIXTREE

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <stdlib.h>

using namespace std;

#define symbol_size 255
#define infinity 100000

class SuffixNode {

public:

  SuffixNode(int parent_in,int label_start_in=-1) : parent(parent_in),label_start(label_start_in) {

    clear_children();

    suffix_link = 0;
    suffix_offset = -1;

    label_end       = -1;
    next_left_leaf  = -1;
    next_right_leaf = -1;
    olink=-1;
  }

  int get_label_length() {
    if(label_start == -1) return 0;

    if(label_end == end_marker) {
      return end_marker_value-label_end;
    }

    return label_end-label_start;
  }

  void clear_children() {
    for(size_t n=0;n<symbol_size;n++) children   [n] = -1;
  }

  void copy_children(SuffixNode &other) {
    for(size_t n=0;n<symbol_size;n++) {
      children[n] = other.children[n];
    }
  }

  void replace_children(size_t old_id,size_t new_id) {
    for(int n=0;n<255;n++) {
      if(children[n] == old_id) children[n] = new_id;
    }
  }

  bool has_children() {
    for(size_t n=0;n<symbol_size;n++) {
      if(children[n] != -1) return true;
    }

    return false;
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

  int get_suffix_offset() {
    if(suffix_offset == -1) return 0;
    if(suffix_offset >=  0) return SuffixNode::end_marker_value-suffix_offset+1; // +1 because marker not updated correctly

    return 0;
  }

  int get_child(int n) { return children[n]; }

  int parent;
  int label_start;
  int label_end  ;
  int children   [symbol_size];
  int suffix_link;
  int suffix_offset;
  int next_left_leaf;
  int next_right_leaf;
  int olink;
  int olink_off;

  static int end_marker;
  static int end_marker_value;
};

int SuffixNode::end_marker = -1;
int SuffixNode::end_marker_value = -1;


class SuffixTree {

public:
  SuffixTree() {
    
    SuffixNode root(0);
    root.suffix_link = 0;
    root.parent = 0;

    store.push_back(root);

    root_node = 0;

    split_distance = 0;
    split_point_node     = 0;
    split_point_position = 0;

    first = true;
    SuffixNode::end_marker_value = -1;
    posrem=0;
  }

  bool exists(vector<char> t) {

    // follow labels from root down, edge labels.

    int current_node = store[0].children[t[0]];
 
    if(current_node == -1) return false;

    cout << "0navigating to: " << current_node << endl;
    size_t search_string_position = 0;

    for(;search_string_position < t.size();) {
      // follow edge label
      cout << "label is: " << store[current_node].label_start << ":" << store[current_node].get_label_end() << endl;
      for(int position=store[current_node].label_start;position <= store[current_node].get_label_end();position++) {
        if(s[position] != t[search_string_position]) { cout << "NOT FOUND" << endl; return false; }
        else {
          cout << "comparison to position: " << position << endl;
          search_string_position++;
          if(search_string_position == t.size()) { cout << "FOUND" << endl; return true; }
        }
      }

      current_node = store[current_node].children[t[search_string_position]];
      cout << "1navigating to: " << current_node << endl;
      if(current_node == -1) return false;
    }

    cout << "NOT FOUND" << endl;
    return false;
  }

  int extend2(int insertion_point,int symbol,int symbol_index_start,int symbol_index_end) {

    cout << "extend2" << endl;
    cout << "Insertion point: " << insertion_point << endl;
    cout << "symbol            : " << symbol << endl;
    cout << "symbol_index_start: " << symbol_index_start << endl;
    cout << "symbol_index_end  : " << symbol_index_end   << endl;

    int label_start = store[insertion_point].label_start;
    int edge_length = store[insertion_point].get_label_length();

    int insert_len = symbol_index_end - symbol_index_start;
    // Check edge label

    cout << "edge_length: " << edge_length << endl;


    // this means we're at the root node, it's kind of special!
    if(store[insertion_point].label_start == -1) {

      // if a child exists, go to it, without consuming
      int child = store[insertion_point].children[s[symbol_index_start]];
      if(child != -1) {
        return extend2(child,0,symbol_index_start,symbol_index_end);
      } else {
        // if it doesn't exist add it.
        SuffixNode sn(insertion_point,symbol_index_start);
        sn.label_start = symbol_index_start;
        store.push_back(sn);
        store[insertion_point].children[s[symbol_index_start]] = store.size()-1;
        return store.size()-1;
      }
    }

    bool dontdoit=false;
    // match at label start position?
    if(edge_length == 0 && s[store[insertion_point].label_start] == s[symbol_index_start]) {
      symbol_index_start++;
      dontdoit=true;
    } else
    if(edge_length == 0 && s[store[insertion_point].label_start] != s[symbol_index_start]) {

      // mismatch at label start position - add new child to parent!

      int parent = store[insertion_point].get_parent();
      int child  = store[parent].get_child(s[symbol_index_start]);
      if(child == -1) {
        // no child here, add one.
        SuffixNode sn(parent,symbol_index_start);
        sn.label_start = symbol_index_start;
        store.push_back(sn);
        store[parent].children[s[symbol_index_start]] = store.size()-1;
        return store.size()-1;
      } else {
        // recurse!
        cout << "THIS IS THE SUCK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
 exit(0);
      //  return extend2(child,0,symbol_index_start,symbol_index_end);
      }


    }

//NEW <=
    // consume edge label
    if(!dontdoit)
    for(int n=0;(n<=edge_length) && (n<=(insert_len));n++) {
      if(s[symbol_index_start+n] != s[label_start+n]) {
        // mismatch on edge label
        cout << "Extend2 condition 1: Mismatch in edge label" << endl;
        cout << "Mismatch points " << symbol_index_start+n << "," << label_start+n << ",   n=" << n << endl;

        // new bit...
        int b_idx = store.size();
        int c_idx = store.size()+1;

        size_t old_parent      = store[insertion_point].parent;
        size_t old_label_start = store[insertion_point].label_start;
        size_t old_label_end   = store[insertion_point].label_end;
        size_t old_suffix_link = store[insertion_point].suffix_link;

        SuffixNode b(insertion_point,0);
        SuffixNode c(insertion_point,0);

        b.copy_children(store[insertion_point]);
        for(size_t i=0;i<255;i++) if(store[insertion_point].children[i] != -1) { store[store[insertion_point].children[i]].parent = b_idx;  }
        store[insertion_point].clear_children();
        store[insertion_point].children[s[old_label_start+n]]    = b_idx;
        store[insertion_point].children[s[symbol_index_start+n]] = c_idx;

        store[insertion_point].label_end = old_label_start+n-1;
        b.label_start = old_label_start+n;
        b.label_end   = old_label_end;
        size_t suffix_len = store[old_suffix_link].get_label_length();
        if(suffix_len <= n) b.suffix_link = old_suffix_link;
                      else {
                        size_t strleft = n;
                        bool end=false;
                        size_t current = old_suffix_link;
                        for(;end==false;) {
                          size_t len = store[current].get_label_length();
                          cout << "len: " << len << endl;
                          cout << "strleft: " << strleft << endl;
                          if(strleft <= len) {b.suffix_link = current; end=true;}
                          strleft -= len;
                          current = store[current].children[s[old_label_start+len]];
                          cout << "***************************************************** DOING THIS THING" << endl;
                        }
                        b.suffix_link = current;
                     } //< something like this...

        c.label_start = symbol_index_start+n;
        c.label_end   = SuffixNode::end_marker;

        store.push_back(b);
        store.push_back(c);
 
        return c_idx;
/*
        int b_idx = store.size();
        int c_idx = store.size()+1;

        size_t old_parent      = store[insertion_point].parent;
        size_t old_label_start = store[insertion_point].label_start;
        size_t old_label_end   = store[insertion_point].label_end;
        size_t old_suffix_link = store[insertion_point].suffix_link;

        store[old_parent].replace_children(insertion_point,b_idx);


        store[insertion_point].parent = b_idx;
        store[insertion_point].label_start = old_label_start+n;
        SuffixNode b(old_parent,0);
        SuffixNode c(b_idx,0);
 
        b.suffix_link = old_suffix_link;
        b.label_start = old_label_start;
        b.label_end   = old_label_start+n-1;

        c.label_start = symbol_index_start+n;
        c.label_end   = SuffixNode::end_marker;

        b.children[s[old_label_start+n]]    = insertion_point;
        b.children[s[symbol_index_start+n]] = c_idx;

        store.push_back(b);
        store.push_back(c);

        return c_idx;*/
      }
    }

    // Edge label matched insertion string completely.
    if((edge_length+1) > insert_len) { cout << "Extend2 condition 1.5: match in edge_label: returning " << insertion_point << endl; return insertion_point; }

    cout << "Extend2 condition 2: checking children" << endl;

    int pos = symbol_index_start + edge_length + 1;

    if(dontdoit) pos--;
    if(pos > symbol_index_end) return insertion_point;

    char child_sym = s[pos];

    // if a child does not exist add
    if(store[insertion_point].children[child_sym] == -1) {
      cout << "Extend2 condition 2b: no children at point past edge label" << endl;
      cout << "                      child_sym: " << static_cast<int>(child_sym) << endl;
      SuffixNode newnode(insertion_point,pos);
      newnode.label_end   = SuffixNode::end_marker;

      store.push_back(newnode);
      int n_idx = store.size()-1;
      store[insertion_point].children[child_sym] = n_idx;
      return n_idx;
    }

    // if a child does exist, recurse
    cout << "recursing, position is now: " << pos << endl;
    return extend2(store[insertion_point].children[child_sym],child_sym,pos,symbol_index_end);
  }

  void insert(char current_symbol) {

    cout << "calling insert: " << current_symbol << endl;

    s.push_back(current_symbol);

    bool first=true;
    int last_node=0;
    //for(int n=s.size()-1;n>=0;n--) {
    for(int n=0;n<s.size();n++) {
      int  posremin;
      bool insertion;
      cout << "inserting: ";
      for(int i=n;i<=s.size()-1;i++) {
        cout << s[i];
      }
      cout << endl;
      int newnode = extend2(0,current_symbol,n,s.size()-1);
      // if(!first) store[newnode].suffix_link = last_node;
      if(!first) store[last_node].suffix_link = newnode;
      last_node = newnode;
      first=false;
    }

    SuffixNode::end_marker_value++;

  }


  void dump() {
    cout << "****************************** Tree dump" << endl;
    for(size_t n=0;n<store.size();n++) {
      cout << "node: " << n << endl;
      cout << "label: " << store[n].label_start << " ";
      if(store[n].label_end == SuffixNode::end_marker) cout << store[n].label_end << "(" << SuffixNode::end_marker_value << ")" << endl;
                                                  else cout << store[n].label_end << endl;
      cout << "suffix_link  : " << store[n].suffix_link   << endl;
      cout << "suffix_offset: " << store[n].suffix_offset << "(" << store[n].get_suffix_offset() << ")" << endl;
      cout << "parent       : " << store[n].parent        << endl;
      for(int i=0;i<symbol_size;i++) {
        if(store[n].children[i] != -1) { cout << "children[" << i << "]:" << store[n].children[i] << endl; }
      }
    }
    cout << "****************************** Tree dump complete" << endl;
  }


  string get_path_label(int n) {
    vector<size_t> parents;
    size_t parent = n;
    for(;parent != 0;) {
      parents.push_back(parent);
      parent = store[parent].parent;
    }

    string my_path_label;
    for(int n=parents.size()-1;n>=0;n--) {
      int start = store[parents[n]].label_start;
      int end   = store[parents[n]].get_label_end();
      for(int i=start;i<=end;i++) {
        my_path_label += s[i];
      }
    }
    return my_path_label;
  }

  void validate_suffix_link(size_t n) {
 
   // get my path label
   // get parent chain to root.

    string my_path_label     = get_path_label(store[n].parent) + s[store[n].label_start];
    string suffix_path_label = get_path_label(store[store[n].suffix_link].parent) + s[store[store[n].suffix_link].label_start];
    
    cout << "validating link from/to: " << n << "," << store[n].suffix_link << endl;
    cout << "labels: " << my_path_label << "," << suffix_path_label << endl;
    if((static_cast<int>(my_path_label.size())-1) > 0)
    for(int n=0;n<my_path_label.size()-1;n++) {
      if(suffix_path_label[n] != my_path_label[n+1]) cout << "****************************************************** SUFFIXLINK DOES NOT VALIDATE" << endl;
    }
  }

  void validate_parent(size_t n) {
    size_t parent =  store[n].parent;

    bool ok=false;
    for(int i=0;i<255;i++) { if(store[parent].children[i] == n) ok = true; }

    if(n == 0) ok = true;
    if(ok != true) cout << "************************************************************************************ ERROR PARENT LINK NOT VALIDATED" << endl;
  }

  void validate_tree() {
    for(size_t n=1;n<store.size();n++) {
      validate_suffix_link(n);
      validate_parent(n);
    }
  }

  vector<char>       s;
  vector<SuffixNode> store;

  bool first;
  int root_node;
  int split_point_node;      ///< Point of last insertion/split in tree (node index)
  int split_point_position;  ///< Point of last insertion/split in tree (label position)
  int split_distance;    ///< distance to last split point
  int current_node;      ///< Current node in tree (active point?)
  int posrem;
};

#endif
