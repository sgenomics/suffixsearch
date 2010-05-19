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

    label_end       = -1;
    next_left_leaf  = -1;
    next_right_leaf = -1;
  }

  SuffixNode() {

    clear_children();

    suffix_link = 0;

    label_start     = -1;
    label_end       = -1;
    next_left_leaf  = -1;
    next_right_leaf = -1;
    parent          = -1;
  }

  int edge_label_length() {
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

  bool has_children() {
    for(size_t n=0;n<symbol_size;n++) {
      if(children[n] != -1) return true;
    }

    return false;
  }

  int get_label_end() {
    if(label_end == end_marker) {
      return end_marker_value;
    }
    return label_end;
  }

  int label_start;
  int label_end  ;
  int children   [symbol_size];
  int parent;
  int suffix_link;
  int next_left_leaf;
  int next_right_leaf;

  static int end_marker;
  static int end_marker_value;
};

int SuffixNode::end_marker = -1;
int SuffixNode::end_marker_value = 0;


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
  }

  void insert_first(char current_symbol) {
    SuffixNode n;

    n.label_start = 0;
    n.label_end   = -1;
    n.suffix_link = 0;
    n.parent      = 0;

    store.push_back(n);

    store[root_node].children[current_symbol] = 1;

    current_node = 1;
    split_point_node = 1;
    split_point_position = 0;

    first = false;
  }

  void insert(char current_symbol) {

    cout << "calling insert: " << current_symbol << endl;

    s.push_back(current_symbol);
    if(first) return insert_first(current_symbol);

    bool insertion=true;
    cout << "split_distance: " << split_distance << endl;

    int c_split_point_node = split_point_node;
    int last_split_point_label_start = store[store[split_point_node].suffix_link].label_start;


    int link     = store[current_node].suffix_link;
    int position = abs(((store[link].label_start - split_distance))-last_split_point_label_start);
    int test     = extend(link,current_symbol,s.size()-1,insertion,position,true);

    if(insertion) {
      current_node = split_point_node;
      position     = split_distance;
    }

    insertion = true;
    for(;(position>=0) && insertion;) {

      cout << "split_distance: " << split_distance << endl;
      int link = store[current_node].suffix_link;

      cout << "insertion: link: " << link << " current_symbol: " << current_symbol << " position: " << split_distance << " insertion: " << insertion << " split_dist: " << split_distance << endl;

      cout << "split_distance             : " << split_distance << endl;
      cout << "label                      : " << store[link].label_start << endl;
      cout << "split_point_node           : " << c_split_point_node << endl;
      cout << "split_point_node.label     : " << store[c_split_point_node].label_start << endl;
      cout << "split_point_node.link      : " << store[c_split_point_node].suffix_link << endl;
      cout << "split_point_node.link.label: " << store[store[c_split_point_node].suffix_link].label_start << endl;

//      cout << "position is: abs(" << store[link].label_start << "-" << split_distance << ")-" << last_split_point_label_start << ")" << endl;
      //int position = split_distance; // abs(((store[link].label_start - split_distance))-last_split_point_label_start);
      cout << "insaneoflex position: " << position << endl;

      int newnode;

      //newnode = extend(link,current_symbol,s.size()-1,insertion,split_distance);
      newnode = extend(link,current_symbol,s.size()-1,insertion,position);

      store[current_node].suffix_link = newnode;
      current_node = newnode;

      cout << "new current_node is : " << current_node << endl;

      if(!insertion) {
         cout << "************************************************** insertion was false" << endl;
      }

      if(insertion) {
        split_point_node = current_node;
        position--;
      }
      cout << "*************************************************************************************************************** DUMP BETA BEGIN" << endl;
      dump();
      cout << "*************************************************************************************************************** DUMP BETA END" << endl;

    }

    if(insertion) {
      split_point_node=current_node;
      split_point_position=0;
      split_distance=0;
    } else split_distance++;
    cout << "split_distance: " << split_distance << endl;

    SuffixNode::end_marker_value++;
  }


  int extend(int insertion_point,int symbol,int symbol_index,bool &insertion,int position=0,bool test=false) {

    cout << "performing extension" << endl;

    cout << "insertion p  : " << insertion_point << endl;
    cout << "label length : " << store[insertion_point].edge_label_length() << endl;
    cout << "position     : " << position << endl;

    // 0. validation check
    if(position > store[insertion_point].edge_label_length()) {
      cout << "didn't take this in to account, how does it happen?" << endl;
      cout << "extend condition 0" << endl;
      // Could account for this by following children past edge label
    }

    // 1. If the edge label is length 0, but a child with this symbol exists, return that child
    if(position == 0) {
      cout << "extend condition 0.1" << endl;

      if(store[insertion_point].label_start != -1) {

        if(s[store[insertion_point].label_start] == symbol) {
          // match at position 0
          cout << "**************************************************** match at position 0 good times" << endl;
          insertion = false;
          return insertion_point;
        } else {
          // mismatch at position 0, need to jump to parent
          cout << "**************************************************** get label end, require validation of new position please" << endl;
          return extend(store[insertion_point].parent,symbol,symbol_index,insertion,store[store[insertion_point].parent].get_label_end(),test);
        }

      } else {
        // non-existance edge_label, go directly to children
        if(store[insertion_point].children[symbol] == -1) {
          // child does not exist add it.
          insertion = true;
          if(!test) {
            cout << "**************************************************** performing position 0 root insertion please validate" << endl;
            SuffixNode newnode(insertion_point,symbol_index);
            store.push_back(newnode);
            store[insertion_point].children[symbol] = store.size()-1;
          }
          return store.size()-1;
        } else {
          insertion = false;
          return store[insertion_point].children[symbol];
        }
      }


    }

    // 1. If the edge label is length 0, but a child with this symbol exists, return that child
    if((store[insertion_point].edge_label_length() == 0) && (store[insertion_point].children[symbol] != -1)) {

      insertion = false;
      cout << "extend condition 1" << endl;
      cout << "insertion child contained: " << store[insertion_point].children[symbol] << endl;
      cout << "returning: " << store[insertion_point].children[symbol] << endl;
      return store[insertion_point].children[symbol];
    }

    // 2. If the edge label is length 0 and there are no children, extend the edge label.
    if((store[insertion_point].edge_label_length() == 0) && (!store[insertion_point].has_children())) {
      insertion = false;
      cout << "extend condition 2" << endl;
      if(!test) {
        if(store[insertion_point].label_end != SuffixNode::end_marker) store[insertion_point].label_end++;  // will this always work?! (ever work?!?)
      }
      return insertion_point;
    }

    // 3. If the edge label is length 0 and there are children, but not with this symbol, add a child with this symbol
    if((store[insertion_point].edge_label_length() == 0) && (store[insertion_point].has_children()) && (store[insertion_point].children[symbol] == -1)) {
      insertion = true;
      if(!test) {
        store.push_back(SuffixNode(insertion_point,symbol_index));
        store[insertion_point].children[symbol] = store.size()-1;
      }
      cout << "extend condition 3" << endl;
      return store.size()-1;
    }

    // 4. If the edge label is length >0 and the edge label matches at this start+position, return this node.
    if((store[insertion_point].edge_label_length() > 0) && (s[store[insertion_point].label_start+position] == symbol)) {
      insertion = false;
      cout << "extend condition 4" << endl;
      return insertion_point;
    }

    // 5. If the edge label if length >0 and the edge label mismatches at start_position, split the node
    if((store[insertion_point].edge_label_length() > 0) && (s[store[insertion_point].label_start+position] != symbol)) {

      insertion = true;
      if(!test) {
	cout << "extend condition 5" << endl;
	cout << "performing funky split" << endl;

	cout << "splitting at position (offset): " << position << endl;
	cout << "in real string terms this is  : " << store[insertion_point].label_start+position << endl;

	int original_label_start = store[insertion_point].label_start;
	int original_label_end   = store[insertion_point].label_end;

	int n1_label_start = original_label_start;
	int n1_label_end   = original_label_start+position-1;

	int n2_label_start = original_label_start+position;
	int n2_label_end   = original_label_end;
   
	int n3_label_start = symbol_index;
	int n3_label_end   = SuffixNode::end_marker;


	char mismatch_symbol = s[original_label_start+position];


	store[insertion_point].label_start = n1_label_start;
	store[insertion_point].label_end   = n1_label_end;

	SuffixNode n2(insertion_point);
	n2.label_start = n2_label_start;
	n2.label_end   = n2_label_end;
	n2.suffix_link = store[insertion_point].suffix_link;
	n2.copy_children(store[insertion_point]);
	store.push_back(n2);
	int n2_idx = store.size()-1;

	SuffixNode n3(insertion_point);
	n3.label_start = n3_label_start;
	n3.label_end   = n3_label_end;
	n3.suffix_link = 0;//root
	store.push_back(n3);
	int n3_idx = store.size()-1;

	store[insertion_point].clear_children();
	store[insertion_point].children[mismatch_symbol] = n2_idx;
	store[insertion_point].children[symbol] = n3_idx;
      }
      return insertion_point;
    }
  }

  void dump() {
    cout << "****************************** Tree dump" << endl;
    for(size_t n=0;n<store.size();n++) {
      cout << "node: " << n << endl;
      cout << "label: " << store[n].label_start << " ";
      if(store[n].label_end == SuffixNode::end_marker) cout << store[n].label_end << "(" << SuffixNode::end_marker_value << ")" << endl;
                                                  else cout << store[n].label_end << endl;
      cout << "suffix_link: " << store[n].suffix_link << endl;
      cout << "parent     : " << store[n].parent << endl;
      for(int i=0;i<symbol_size;i++) {
        if(store[n].children[i] != -1) { cout << "children[" << i << "]:" << store[n].children[i] << endl; }
      }
    }
    cout << "****************************** Tree dump complete" << endl;
  }

  vector<char>       s;
  vector<SuffixNode> store;

  bool first;
  int root_node;
  int split_point_node;      ///< Point of last insertion/split in tree (node index)
  int split_point_position;  ///< Point of last insertion/split in tree (label position)
  int split_distance;    ///< distance to last split point
  int current_node;      ///< Current node in tree (active point?)
};

#endif
