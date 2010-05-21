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

  SuffixNode() {

    clear_children();

    suffix_link = 0;
    suffix_offset = -1;

    label_start     = -1;
    label_end       = -1;
    next_left_leaf  = -1;
    next_right_leaf = -1;
    parent          = -1;
    olink=-1;
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

  int get_suffix_offset() {
    if(suffix_offset == -1) return 0;
    if(suffix_offset >=  0) return SuffixNode::end_marker_value-suffix_offset+1; // +1 because marker not updated correctly
  }

  int label_start;
  int label_end  ;
  int children   [symbol_size];
  int parent;
  int suffix_link;
  int suffix_offset;
  int next_left_leaf;
  int next_right_leaf;
  int olink;

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

  bool exists(vector<char> t) {

    // follow labels from root down, edge labels.

    int current_node = store[0].children[t[0]];
 
    if(current_node == -1) return false;

    cout << "0navigating to: " << current_node << endl;
    int search_string_position = 0;

    for(;search_string_position < t.size();) {
      // follow edge label
      cout << "label is: " << store[current_node].label_start << ":" << store[current_node].get_label_end() << endl;
      for(int position=store[current_node].label_start;position <= store[current_node].get_label_end();position++) {
        if(s[position] != t[search_string_position]) return false;
        else {
          cout << "comparison to position: " << position << endl;
          search_string_position++;
          if(search_string_position == t.size()) return true;
        }
      }

      current_node = store[current_node].children[t[search_string_position]];
      cout << "1navigating to: " << current_node << endl;
      if(current_node == -1) return false;
    }
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

    bool insertion=false;
    cout << "split_distance: " << split_distance << endl;

    int position = split_distance; 

    cout << "test position is: " << position << endl;
    cout << "TEST split_point_node: " << split_point_node << endl;
    //cout << "TEST node            : " << store[split_point_node].suffix_link << endl;
    cout << "TEST node            : " << store[store[split_point_node].suffix_link].suffix_link << endl;
    cout << "TEST split_distance  : " << split_distance << endl;

    int testnode     = store[store[split_point_node].suffix_link].suffix_link;
    int testnode_off = store[store[split_point_node].suffix_link].get_suffix_offset();
    cout << "TEST OFFSET RETURNED: " << testnode_off << endl;
    if(s[store[testnode].label_start+split_distance+testnode_off] != current_symbol) {
//////////////////////////    if(s[store[store[split_point_node].suffix_link].label_start+split_distance] != current_symbol) {
      cout << "TEST comparison mismatched" << endl;
      current_node = store[split_point_node].suffix_link;
      position     = split_distance;
    } else {cout << "TEST comparison matched" << endl;}

    insertion = true;
    cout << "split_distance: " << split_distance << endl;
    bool first=true;

  /////////////////////////////////////  current_node = split_point_node;
    current_node = store[split_point_node].suffix_link;
    bool any_insert=false;
    bool first_insertion=true;
    //for(;(position>=0) && insertion;) {
    //position = store[current_node].edge_label_length()-position;
    //if(current_symbol == '$') position++;

    for(int n=0;n<store.size();n++) store[n].olink = -1; // AAAAAAAAAAAARRRRGGGHHHH NON LINEAR!

    for(;position>=0;) {

      int link;

      if(store[current_node].olink == -1) store[current_node].olink = store[current_node].suffix_link;
      link = store[current_node].olink; 
      if(store[link].olink == -1) store[link].olink = store[link].suffix_link;
    //  link = store[current_node].suffix_link; 
      
      cout << "split_distance             : " << split_distance << endl;
      cout << "label                      : " << store[link].label_start << endl;

      cout << "insaneoflex position: " << position << endl;

      int newnode;

      cout << "*** insertion: link: " << link << " current_symbol: " << current_symbol << " position: " << position+store[current_node].get_suffix_offset() << " insertion: " << insertion << " split_dist: " << split_distance << endl;
  
      int n2;
      newnode = extend(link,current_symbol,s.size()-1,insertion,position+store[current_node].get_suffix_offset(),false,&n2);

      int oldlink = link;
      store[current_node].suffix_link = newnode;///////////////////////////////////////////////////////////
      store[current_node].suffix_offset = -1;


      cout << "new current_node is : " << current_node << endl;

      if(!insertion) {
         cout << "************************************************** insertion was false, incremented split_distance" << endl;
      }

      position--;
 /////     current_node = oldlink;
      if(insertion && first_insertion) {
        split_point_node=current_node;
        split_point_position=0;
        split_distance=0;
        any_insert=true;
        first_insertion=false;
      }
      cout << "*************************************************************************************************************** DUMP BETA BEGIN" << endl;
      dump();
      cout << "*************************************************************************************************************** DUMP BETA END" << endl;
      first=false;
    }

    split_distance++;
    if(any_insert) split_distance=0;

    // insert single character.
    if(store[0].children[current_symbol] == -1) {
      SuffixNode newnode(0,s.size()-1);
      store.push_back(newnode);
      store[0].children[current_symbol] = store.size()-1;
 
      //
      store[current_node].suffix_link = store.size()-1;
      current_node = store.size()-1;
    }

    // store[current_node].suffix_link = ?!?
 ////   if(any_insert) split_distance=0;

    cout << "split_distance: " << split_distance << endl;
    SuffixNode::end_marker_value++;
  }


  int extend(int insertion_point,int symbol,int symbol_index,bool &insertion,int position=0,bool test=false,int *n2=0) {

    cout << "performing extension" << endl;

    cout << "insertion p  : " << insertion_point << endl;
    cout << "label length : " << store[insertion_point].edge_label_length() << endl;
    cout << "position     : " << position << endl;

    if(insertion_point == 0) {
      insertion_point = store[0].children[s[symbol_index-position]];

      if(insertion_point == -1) {

       //  if(position == 0) position = 1;

         // there is no child with this symbol, add one!
         SuffixNode newnode(0,(symbol_index-position)); // -position (make this generalise was +1
         store.push_back(newnode);
         int nidx = store.size()-1;
         store[0].children[s[(symbol_index-position)]] = nidx; // was +1
         //store[insertion_point].children[symbol] = nidx;
         cout << "extend condition -0b" << endl;
         cout << "extend condition -0b, insertion_point: " << 0 << endl;
         cout << "extend condition -0b, symbol: " << symbol << endl;
         cout << "extend condition -0b, symbol_index: " << symbol_index << endl;
         cout << "extend condition -0b, position: " << position << endl;
         cout << "extend condition -0b, insertion node: " << s[(symbol_index-position)] << endl;
         cout << "extend condition -0b, insertion node idx: " << (symbol_index-position) << endl;

         return nidx;
      }
    }

    // 0. validation check
    if(position > store[insertion_point].edge_label_length()) {

      position = position - store[insertion_point].edge_label_length();
      if(position == 0) {
        
        // there is a child here at symbol?
       if(store[insertion_point].children[symbol] != -1) {
         cout << "extend condition 0a" << endl;
         return store[insertion_point].children[symbol];
       } else {
         // there is no child with this symbol, add one!
         SuffixNode newnode(insertion_point,symbol_index);
         store.push_back(newnode);
         int nidx = store.size()-1;
         store[insertion_point].children[symbol] = nidx;
         cout << "extend condition 0b" << endl;
         return nidx;
       }
      }

      if(position >= 1) {
        // there is a child here at symbol?
       //if(store[insertion_point].children[s[symbol_index-1]] != -1) {
       if(store[insertion_point].children[symbol] != -1) {
         cout << "extend condition 0c" << endl;
         cout << "extend condition 0c, insertion_point: " << insertion_point << endl;
         return extend(store[insertion_point].children[s[symbol_index-position]],symbol,symbol_index,insertion,position,test,n2);
       } else {
         // there is no child with this symbol, add one!
         SuffixNode newnode(insertion_point,symbol_index-position+1); // -position (make this generalise
         store.push_back(newnode);
         int nidx = store.size()-1;
         store[insertion_point].children[s[symbol_index-position+1]] = nidx;
         //store[insertion_point].children[symbol] = nidx;
         cout << "extend condition 0d" << endl;
         cout << "extend condition 0d, insertion_point: " << insertion_point << endl;
         cout << "extend condition 0d, symbol: " << symbol << endl;
         return nidx;
       }
         
      }

      cout << "didn't take this in to account, how does it happen?" << endl;
      cout << "extend condition 0" << endl;
      // Could account for this by following children past edge label
    }

    // 1. If the edge label is length 0, but a child with this symbol exists, return that child
    if(position == 0) {
      cout << "extend condition 0.1" << endl;

      cout << "insertion point: " << insertion_point << endl;
      if(store[insertion_point].label_start != -1) {

        if(s[store[insertion_point].label_start+position] == symbol) {
          // match at position 0
          cout << "extend condition 0.1a" << endl;
          cout << "**************************************************** match at position 0 good times" << endl;
          insertion = false;
          return insertion_point;
        } else {
          // mismatch at position 0, need to jump to parent
          cout << "**************************************************** get label end, require validation of new position please" << endl;
          cout << "extend condition 0.1b" << endl;
          //return extend(store[insertion_point].parent,symbol,symbol_index,insertion,store[store[insertion_point].parent].get_label_end(),test);
          //return extend(store[insertion_point].parent,symbol,symbol_index,insertion,position,test);

          if(store[store[insertion_point].parent].children[symbol] != -1) return extend(store[store[insertion_point].parent].children[symbol],symbol,symbol_index,insertion,position,test);
                                                                     else return extend(store[insertion_point].parent                        ,symbol,symbol_index,insertion,position,test);

//          return extend(store[insertion_point].parent,symbol,symbol_index,insertion,position,test);
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
          cout << "extend condition 0.1c" << endl;
          return store.size()-1;
        } else {
          insertion = false;
          cout << "extend condition 0.1d" << endl;
          return extend(store[insertion_point].children[symbol],symbol,symbol_index,insertion,position,test);
          // return store[insertion_point].children[symbol];
        }
      }
    }

    // 1. If the edge label is length 0, but a child with this symbol exists, return that child
    if((store[insertion_point].edge_label_length() == 0) && (store[insertion_point].children[symbol] != -1)) {

      insertion = false;
      cout << "extend condition 1" << endl;
      cout << "extend condition 1 position: " << position << endl;
      // cout << "insertion child contained: " << store[insertion_point].children[symbol] << endl;
      // cout << "returning: " << store[insertion_point].children[symbol] << endl;
      return extend(store[insertion_point].children[symbol],symbol,symbol_index,insertion,position,test);
      // return store[insertion_point].children[symbol];
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
      cout << "extend condition 4: position: " << position << endl;
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
        cout << "insertion_point is            : " << insertion_point << endl;

	int original_label_start = store[insertion_point].label_start;
	int original_label_end   = store[insertion_point].label_end;

	int n1_label_start = original_label_start;
	int n1_label_end   = original_label_start+position-1;

	int n2_label_start = original_label_start+position;
	int n2_label_end   = original_label_end;
   
	int n3_label_start = symbol_index;
	int n3_label_end   = SuffixNode::end_marker;

       if((n1_label_end != -1) && (n1_label_start > n1_label_end)) cout << "******************************************************************************************************************************** N1 LABEL ERR" << endl;
       if((n2_label_end != -1) && (n2_label_start > n2_label_end)) cout << "******************************************************************************************************************************** N2 LABEL ERR" << endl;
       if((n3_label_end != -1) && (n3_label_start > n3_label_end)) cout << "******************************************************************************************************************************** N3 LABEL ERR" << endl;

        cout << "original_label_start: " << original_label_start << endl;
        cout << "original_label_end  : " << original_label_end   << endl;
        cout << "position            : " << position             << endl;


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

        // update parenthood of children
        for(int n=0;n<symbol_size;n++) {
          if(n2.children[n] != -1) {
            store[n2.children[n]].parent = n2_idx;
          }
        }


	SuffixNode n3(insertion_point);
	n3.label_start = n3_label_start;
	n3.label_end   = n3_label_end;
	n3.suffix_link = 0;//root
        n3.suffix_offset = symbol_index;//position;//symbol_index;
	// n3.suffix_link = store[insertion_point].suffix_link; /// WEIRDNESS TRY
	store.push_back(n3);
	int n3_idx = store.size()-1;

	store[n2_idx].suffix_link = n3_idx; // WEIRDNESS TRY

	store[insertion_point].clear_children();
	store[insertion_point].children[mismatch_symbol] = n2_idx;
	store[insertion_point].children[symbol] = n3_idx;
        return n2_idx;//insertion_point; //return n2_idx;// was n3_idx
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
      cout << "suffix_link  : " << store[n].suffix_link   << endl;
      cout << "suffix_offset: " << store[n].suffix_offset << "(" << store[n].get_suffix_offset() << ")" << endl;
      cout << "parent       : " << store[n].parent        << endl;
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
