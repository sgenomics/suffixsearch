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
    for(int64_t n=0;n<symbol_size;n++) children   [n] = -1;
  }

  void copy_children(SuffixNode &other) {
    for(int64_t n=0;n<symbol_size;n++) {
      children[n] = other.children[n];
    }
  }

  void replace_children(int64_t old_id,int64_t new_id) {
    for(int n=0;n<255;n++) {
      if(children[n] == old_id) children[n] = new_id;
    }
  }

  bool has_children() {
    for(int64_t n=0;n<symbol_size;n++) {
      if(children[n] != -1) return true;
    }

    return false;
  }

  int first_child() {
    for(int64_t n=0;n<symbol_size;n++) {
      if(children[n] != -1) return n;
    }

    return -1;
  }

  int find_child(int c) {
    for(int64_t n=0;n<symbol_size;n++) {
      if(children[n] == c) return n;
    }

    return -1;
  }

  int child_count() {
    int i=0;

    for(int64_t n=0;n<symbol_size;n++) {
      if(children[n] != -1) ++i;
    }

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
    split_count = 0;
    store.push_back(root);

    root_node = 0;

    split_distance = 0;
    split_point_node     = 0;
    split_point_position = 0;

    SuffixNode::end_marker_value = -1;
    posrem=0;
    first_insert = true;
    first_node = 0;
  }

  bool exists(vector<char> t) {

    // follow labels from root down, edge labels.

    int current_node = store[0].children[t[0]];
 
    if(current_node == -1) return false;

    //cout << "0navigating to: " << current_node << endl;
    int64_t search_string_position = 0;

    for(;search_string_position < t.size();) {
      // follow edge label
      //cout << "label is: " << store[current_node].label_start << ":" << store[current_node].get_label_end() << endl;
      for(int position=store[current_node].label_start;position <= store[current_node].get_label_end();position++) {
        if(s[position] != t[search_string_position]) { return false; }
        else {
          //cout << "comparison to position: " << position << endl;
          search_string_position++;
          if(search_string_position == t.size()) { return true; }
        }
      }

      current_node = store[current_node].children[t[search_string_position]];
      //cout << "1navigating to: " << current_node << endl;
      if(current_node == -1) return false;
    }

    //cout << "NOT FOUND" << endl;
    return false;
  }

  int extend2(int insertion_point,int symbol_index_start,int symbol_index_end,bool &split,int &fnode,int &fpos) {

    cout << "extend2, insertion_point   : " << insertion_point << endl;
    cout << "extend2, symbol_index_start: " << symbol_index_start << endl;
  //  cout << "extend2, label_start       : " << store[insertion_point].label_start << endl;

    fnode = insertion_point;
    fpos  = symbol_index_start;

    int label_start = store[insertion_point].label_start;
    int edge_length = store[insertion_point].get_label_length();

    int insert_len = symbol_index_end - symbol_index_start;
    // Check edge label

    // this means we're at the root node, it's kind of special!
    if(store[insertion_point].label_start == -1) {

      // if a child exists, go to it, without consuming
      int child = store[insertion_point].children[s[symbol_index_start]];
      if(child != -1) {
        return extend2(child,symbol_index_start,symbol_index_end,split,fnode,fpos);
      } else {
        // if it doesn't exist add it.
        SuffixNode sn(insertion_point,symbol_index_start);
        sn.label_start = symbol_index_start;
        sn.suffix_link = 0;
        store.push_back(sn);
        split = true;
        store[insertion_point].children[s[symbol_index_start]] = store.size()-1;
        //cout << "1 ADD NODE: " << store.size()-1 << endl;
        //cout << "extend2 endpoint 1" << endl;
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
        split=true;
        store.push_back(sn);
        store[parent].children[s[symbol_index_start]] = store.size()-1;
        //cout << "2ADD NODE: " << store.size()-1 << endl;
        //cout << "extend2 endpoint 2" << endl;
        return store.size()-1;
      } else {
        // Should really never reach this point.
        exit(0);
      }
    }

    // consume edge label
    if(!dontdoit)
    for(int n=0;(n<=edge_length) && (n<=(insert_len));n++) {
      if(s[symbol_index_start+n] != s[label_start+n]) {
        // mismatch on edge label
        //cout << "Extend2 condition 1: Mismatch in edge label" << endl;
        //cout << "Mismatch points " << symbol_index_start+n << "," << label_start+n << ",   n=" << n << endl;

        // new bit...
        int b_idx = store.size();
        int c_idx = store.size()+1;

        int64_t old_parent      = store[insertion_point].parent;
        int64_t old_label_start = store[insertion_point].label_start;
        int64_t old_label_end   = store[insertion_point].label_end;
        int64_t old_suffix_link = store[insertion_point].suffix_link;

        SuffixNode b(insertion_point,0);
        SuffixNode c(insertion_point,0);
        b.label_start = old_label_start;
        b.label_end   = old_label_start+n-1;

        c.label_start = symbol_index_start+n;
        c.label_end   = SuffixNode::end_marker;

        store[insertion_point].label_start = old_label_start+n;

        int old_parent_child_symbol = store[old_parent].find_child(insertion_point); // TODO: make constant time please?

        store[old_parent].children[old_parent_child_symbol] = b_idx;

        b.children[s[old_label_start+n]] = insertion_point;
        b.children[s[symbol_index_start+n]] = c_idx;


        store[insertion_point].parent = b_idx;
        c.parent = b_idx;
        b.parent = old_parent;
        b.suffix_link = 0;// (this is pointed after the next insertion in insert)

        //cout << "***************************************************** 3ADD NODE: " << b_idx << endl;
        //cout << "***************************************************** 3ADD NODE: " << c_idx << endl;
        split=true;
        store.push_back(b);
        store.push_back(c);
        //cout << "extend2 endpoint 3" << endl;
        return c_idx;
      }
    }

    // Edge label matched insertion string completely.
    if((edge_length+1) > insert_len) { 
      split=false;
      //cout << "extend2 endpoint 4" << endl;
      return insertion_point;
    }

    //cout << "Extend2 condition 2: checking children" << endl;

    int pos = symbol_index_start + edge_length + 1;

    if(dontdoit) pos--;
    if(pos > symbol_index_end) {
      //cout << "extend2 endpoint 5" << endl;
      return insertion_point;
    }

    char child_sym = s[pos];

    // if a child does not exist add
    if(store[insertion_point].children[child_sym] == -1) {
      //cout << "Extend2 condition 2b: no children at point past edge label" << endl;
      //cout << "                      child_sym: " << static_cast<int>(child_sym) << endl;
      SuffixNode newnode(insertion_point,pos);
      newnode.label_end   = SuffixNode::end_marker;

      split=true;
      store.push_back(newnode);
      int n_idx = store.size()-1;
      //cout << "***************************************************** 4ADD NODE: " << n_idx << endl;
      store[insertion_point].children[child_sym] = n_idx;
      //cout << "extend2 endpoint 6" << endl;
      return n_idx;
    }

    // if a child does exist, recurse
    cout << "recursing, position is now: " << pos << endl;
    return extend2(store[insertion_point].children[child_sym],pos,symbol_index_end,split,fnode,fpos);
  }

  void insert(char current_symbol) {

    s.push_back(current_symbol);

    int last_node=0;
    int last_node_sl=0;
    vector<vector<int64_t> > doall;
    bool nosplitins = true;

    //cout << "inst: ";
    string ins_str;
    for(int i=0;i<=s.size()-1;i++) {
      ins_str += s[i];
    }
    //cout << ins_str;
    //cout << endl;

    int label_distance = get_path_label(first_node).size() - get_path_label(store[first_node].parent).size()-1;

    last_node = first_node;
    SuffixNode::end_marker_value++;
    bool first=true;
    bool split=false;
    int  new_split_count = split_count;

    int predict_node = 0;
    int predict_pos = 0;
    for(int n=0;n<s.size();n++) {
      int  posremin;
      bool insertion;
dump();
      string ins_str;
      cout << "inserting: ";
      for(int i=n;i<=s.size()-1;i++) {
        cout << s[i];
        ins_str += s[i];
      }
      cout << endl;

/*
      string s_path;
      if(first_insert) cout << "0insertion path is: " << endl; else
      if(first       ) { cout << "1insertion path is: "; s_path = get_path_label(first_node); }
                  else { cout << "2insertion path is: "; s_path = get_path_label(store[last_node].suffix_link); }
      cout << s_path << endl;
*/

      last_node_sl = store[last_node].suffix_link;


      int newnode;

      bool last_split=split;
      split=false;

      predict_node = store[last_node].suffix_link;
      if(first) predict_node = first_node;

      int l_predict_pos = predict_pos;
      if(first) {cout << "pred1" << endl; predict_pos  = (s.size()-1)-store[predict_node].get_label_length_r()+1;} 
      if(store[predict_node].parent       == 0) {cout << "pred2" << endl; predict_pos = n;} 
 //     if(store[predict_node].suffix_link  == 0) {cout << "pred3" << endl; predict_pos = n;}

      // Now need to perform 'canonisation' analog.
      int n_label_length = store[last_node]   .get_label_length();
      int s_label_length = store[predict_node].get_label_length();


      cout << "n_label_length: " << n_label_length << endl;
      cout << "s_label_length: " << s_label_length << endl;
      cout << "pre  -can predict_node: " << predict_node << endl;
      cout << "pre  -can predict_pos : " << predict_pos << endl;

// condition is wrong!

      int distance_in_current = (s.size()-1) - (l_predict_pos+1);
      int label_left          = distance_in_current;//(store[last_node].get_label_length() - distance_in_current);
      cout << "l_predict_pos: " << l_predict_pos << endl;
      cout << "last_node: " << last_node << endl;
      cout << "distance_in_current: " << distance_in_current << endl;
      cout << "label_left: " << label_left << endl;
      cout << " sl length: " << store[predict_node].get_label_length_r() << endl;
      cout << " sl start: " << store[predict_node].label_start << endl;
      cout << " sl end  : " << store[predict_node].label_end << endl;
      if((store[predict_node].parent != 0) && (store[predict_node].suffix_link != 0))
      for(;label_left > store[predict_node].get_label_length_r();) {
        cout << "can loop" << endl;
        predict_node  = store[predict_node].parent;
    //    predict_pos  += store[predict_node].get_label_length_r();
        label_left -= (store[predict_node].get_label_length_r() + 1);
      }

      predict_pos  = (s.size()-1)-store[predict_node].get_label_length_r();
      if(n  == (s.size()-1)) predict_node = 0;
      if(store[predict_node].parent       == 0) {cout << "pred2a" << endl; predict_pos = n;} 
 //     if(store[predict_node].suffix_link  == 0) {cout << "pred3a" << endl; predict_pos = n;}

      cout << "post-can predict_node: " << predict_node << endl;
      cout << "post-can predict_pos : " << predict_pos << endl;


      int fnode = 0;
      int fpos  = 0;
      newnode = extend2(0,n,s.size()-1,split,fnode,fpos); // BRUTE
//      newnode = extend2(predict_node,predict_pos,s.size()-1,split,fnode,fpos);

      if(first) cout << "************ FFFFIRRRRSSSSSSSTTTTTTT" << endl;
      if(fnode != predict_node) cout << "********************************************************** NODE PREDICTION FAILURE " << predict_node << "!=" << fnode << endl;
      if(fpos  != predict_pos ) cout << "**********************************************************  POS PREDICTION FAILURE " << predict_pos  << "!=" << fpos  << endl;


      if(split == true) {
      //  cout << "************************************ split located" << endl;
        new_split_count = 0;
      }

      if((!first) && split) {
        store[last_node].suffix_link = newnode;
//        cout << "0SETLINK: " << last_node << " TO " << newnode << endl;
      }

      if((!first) && last_split) {
        store[store[last_node].parent].suffix_link = store[newnode].parent;
        //cout << "1SETLINK: " << store[last_node].parent << " TO " << store[newnode].parent << endl;
      }

      if(first) {
        first_node = newnode;
        cout << "first_node now: " << first_node << endl;
      }
      
      last_node = newnode; // was newnode
      first=false;
      first_insert=false;

      if(last_split && (!split)) {
        break;
      }
    }
    dump();

    if(split) {
      //cout << "LAST WAS SPLIT: " << store[last_node].parent << endl;
      store[last_node].suffix_link = 0;
      store[store[last_node].parent].suffix_link = 0;
    }

    split_count = new_split_count+1;
  }

  void dump() {
    cout << "****************************** Tree dump" << endl;
    for(int64_t n=0;n<store.size();n++) {
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

    if(n==0) return "";

    vector<int64_t> parents;
    int64_t parent = n;
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

  string get_label(int n) {
    if(n==0) return "";
    int start = store[n].label_start;
    int end   = store[n].get_label_end();
    string my_path_label;
    for(int i=start;i<=end;i++) {
      my_path_label += s[i];
    }
    return my_path_label;
  }

  void validate_suffix_link(int64_t n) {
 
   // get my path label
   // get parent chain to root.

    string my_path_label     = get_path_label(n) ;
    string suffix_path_label = get_path_label(store[n].suffix_link);
  
    cout << "validating link from/to: " << n << "," << store[n].suffix_link << " labels: " << my_path_label << "," << suffix_path_label << endl;
    if((static_cast<int>(my_path_label.size())-1) > 0)
    if((suffix_path_label.size()) < ((my_path_label.size()-1))) {
        cout << "********************************************* SUFFIXLINK DOES NOT VALIDATE, SIZE ERROR1" << endl;
    } else
    if((suffix_path_label.size()) != ((my_path_label.size()-1))) {
        cout << "********************************************* SUFFIXLINK DOES NOT VALIDATE, SIZE ERROR2" << endl;
    } else
    for(int n=0;n<my_path_label.size()-1;n++) {
      if(suffix_path_label[n] != my_path_label[n+1]) cout << "****************************************************** SUFFIXLINK DOES NOT VALIDATE" << endl;
    }
  }

  void validate_parent(int64_t n) {
    int64_t parent =  store[n].parent;

    bool ok=false;
    for(int i=0;i<255;i++) { if(store[parent].children[i] == n) ok = true; }

    if(n == 0) ok = true;
    if(ok != true) cout << "************************************************************************************ ERROR PARENT LINK NOT VALIDATED" << endl;
  }

  void validate_tree() {
    for(int64_t n=1;n<store.size();n++) {
      validate_suffix_link(n);
      validate_parent(n);
    }
  }

  vector<char>       s;
  vector<SuffixNode> store;

  int root_node;
  int split_point_node;      ///< Point of last insertion/split in tree (node index)
  int split_point_position;  ///< Point of last insertion/split in tree (label position)
  int split_distance;        ///< distance to last split point
  int current_node;          ///< Current node in tree (active point?)
  int posrem;
  bool first_insert;
  int64_t first_node;
  int64_t split_count;
};

#endif
