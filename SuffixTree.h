#ifndef SUFFIXTREE
#define SUFFIXTREE

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <stdlib.h>
#include "SearchStore.h"
#include "Transcode.h"
#include "TranscodingStore.h"

using namespace std;

#define symbol_size  45
#define final_symbol 39

class SuffixNode {

public:

  SuffixNode(int parent_in,int label_start_in,int depth_in) : parent(parent_in),label_start(label_start_in), depth(depth_in) {

    clear_children();

    suffix_link = 0;

    label_end       = -1;
    next_left_leaf  = -1;
    next_right_leaf = -1;
  }

  bool isleaf() {
    if(label_end == end_marker) return true;
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
    for(int64_t n=0;n<symbol_size;n++) children   [n] = -1;
  }

  void copy_children(SuffixNode &other) {
    for(int64_t n=0;n<symbol_size;n++) {
      children[n] = other.children[n];
    }
  }

  void replace_children(int64_t old_id,int64_t new_id) {
    for(int n=0;n<symbol_size;n++) {
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

  int get_child(int n) { return children[n]; }

  int parent;
  int label_start;
  int label_end  ;
  int children   [symbol_size];
  int suffix_link;
  int next_left_leaf;
  int next_right_leaf;
  int depth;

  static int end_marker;
  static int end_marker_value;
  static int root;
};



class SuffixTree {

public:
  SuffixTree() {
    
    SuffixNode root(0,-1,-1);
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
    first_non_leaf_node = 0;
    first_non_leaf_n = 0;
  }

  void process_positions() {
    process_right_positions();
cout << "processed right positions" << endl;
    process_left_positions();
cout << "processed left positions" << endl;
  }

  void process_right_positions() {

    // 1. in-order traversal

    deque<int> nodestack;
    deque<int> childstack;

    nodestack .push_back(-1);
    childstack.push_back(-1);

int alphabet_size = symbol_size-1;
    nodestack .push_back(SuffixNode::root);
    childstack.push_back(alphabet_size);

    int lastleaf = -1;
    for(;;) {

      if(childstack.size() == 0) break;
      int current_node;
      current_node  = nodestack.back();
      nodestack.pop_back();
      int current_child;
      current_child = childstack.back();
      childstack.pop_back(); 

      if(current_node == -1) break;
      int oldl = store[current_node].next_right_leaf;
      store[current_node].next_left_leaf = lastleaf;

      if(current_child < 0) {
        nodestack .pop_back();
        childstack.pop_back();

        current_node  = nodestack.back();
        nodestack.pop_back();
        current_child = childstack.back();
        childstack.pop_back(); 
      } else {

        bool stop = false;
	nodestack .push_back(current_node);
        childstack.push_back(alphabet_size);
        bool first_pass = false;
        if(current_child == (alphabet_size)) first_pass=true; // should be symbol_size-1?
        for(;stop == false;) {

	  int nextchild = -1;
          if(current_child >= 0) nextchild = store[current_node].children[current_child];

          if(current_child < 0) {
            stop = true;
            nodestack.pop_back();
            childstack.pop_back();
            if(first_pass) { 
              store[current_node]. next_left_leaf  = oldl;
              store[current_node]. next_right_leaf = lastleaf;
              lastleaf=current_node;
            }
          } else
	  if(nextchild != -1) {
	    nodestack .pop_back();
	    childstack.pop_back();
	    if((current_child-1) >= 0) {
              nodestack .push_back(current_node);
	      childstack.push_back(current_child-1);
            }
	    nodestack .push_back(nextchild);
	    childstack.push_back(alphabet_size);
	    stop = true;
	  } else {
            current_child--;
	  }
        }
      }
    }
  }

  void process_left_positions() {

    // 1. in-order traversal

    deque<int> nodestack;
    deque<int> childstack;

    nodestack .push_back(-1);
    childstack.push_back(-1);

    nodestack .push_back(SuffixNode::root);
    childstack.push_back(0);

    int lastleaf = -1;

    int alphabet_size = symbol_size-1;
    for(;;) {

      int current_node  = nodestack.back();
      nodestack.pop_back();
      int current_child = childstack.back();
      childstack.pop_back(); 

      if(current_node == -1) break;

      //store[current_node]. next_left_leaf = lastleaf;
      int oldr = store[current_node].next_right_leaf;
      store[current_node].next_right_leaf = lastleaf;

      if(current_child+1 >= alphabet_size) {
        nodestack .pop_back();
        childstack.pop_back();

        current_node  = nodestack.back();
        nodestack.pop_back();
        current_child = childstack.back();
        childstack.pop_back(); 
      } else {

        bool stop = false;
	nodestack .push_back(current_node);
        childstack.push_back(0);

        bool first_pass = false;
        if(current_child == 0) first_pass=true;
        for(;stop == false;) {

	  int nextchild = store[current_node].children[current_child];
   
          if(current_child >= alphabet_size) {
            stop = true;
            nodestack.pop_back();
            childstack.pop_back();
            if(first_pass) { 
              store[current_node]. next_right_leaf = oldr; 
              store[current_node]. next_left_leaf = lastleaf; 
              lastleaf=current_node;
            }
          }
 
	  if(nextchild != -1) {
	    nodestack .pop_back();
	    childstack.pop_back();
	    nodestack .push_back(current_node);
	    childstack.push_back(current_child+1);
	    nodestack .push_back(nextchild);
	    childstack.push_back(0);
	    stop = true;
	  } else {
            current_child++;
	  }
        }
      }
    }
  }

  int find_tree_position(vector<char> ss) {
    // follow labels from root down, edge labels.

    int current_node = store[SuffixNode::root].children[ss[0]];
    char label = ss[1];
 
    int search_string_position = 0;
    cout << "current_node: " << current_node << endl;
    if(current_node == -1) return -1;
    for(;search_string_position < ss.size();) {
      // follow edge label
      for(int position=store[current_node].label_start;position <= store[current_node].get_label_end();position++) {
    //    cout << "check pos: " << position << "," << s[position] << "," << ss[search_string_position] << endl;
        if(s[position] != ss[search_string_position]) {return -1;}
        else {
          search_string_position++;
          if(search_string_position == ss.size()) {
            return current_node;
          }
        }
      }

      label = ss[search_string_position];

      current_node = store[current_node].children[label];
      cout << "current_node: " << current_node << endl;
      if(current_node == -1) return -1;
    }
  }

  string get_substr(int start,int end) {
    string res;
cout << "start:" << start << endl;
cout << "end  :" << end << endl;
    if(start > s.size()) return res;
    if(start > end) return res;
    cout << "s.size():" << s.size() << endl;
    if(end > s.size()) end = s.size();
    for(int pos=start;pos<=end;pos++) {
      if((pos < s.size()) && (pos >= 0)) res += s.get_uncoded(pos);
     // if((pos < s.size()) && (pos >= 0)) res += s[pos];
    }

    return res;
  }

  vector<int> all_occurs(vector<char> ss,size_t max_hits=-1) {

    vector<int> res;

    cout << "all_occurs ss: ";
    for(int n=0;n<ss.size();n++) cout << ss[n];
    for(int n=0;n<ss.size();n++) ss[n] =  transcoder.convert(ss[n]);
    cout << endl;
    int p = find_tree_position(ss);

    cout << "string end node was: " << p << endl;
    if(p == -1) {
      return res;
    }

    // grab left and right...

    int nl = store[p].next_left_leaf;
    int nr = store[p].next_right_leaf;

    if(store[p].isleaf()) {
      res.push_back(s.size()-store[p].get_depth());
      return res;
    }

    // itterate from nl until we reach nr

    int c = nl;
    bool stop=false;
    for(;stop==false;) {

      cout << "current: " << c << endl;
      if(c==nr) { stop=true; }

      bool nochild=true;
      if(store[c].label_start != -1) { res.push_back(s.size()-store[c].get_depth()); nochild=false; }//TODO: err somehow convert this back in to correct location?!?

      if(nochild==true) {cout << "there are no children" << endl;}
                   else {cout << "there were the children" << endl; }

      c = store[c].next_right_leaf;

      if(res.size() > max_hits) return res;
    }

    cout << "res size: " << res.size() << endl;
    return res;
  }

  bool exists(vector<char> t) {
 
    for(size_t n=0;n<t.size();n++) t[n] = transcoder.convert(t[n]);

    int res = exists_node(t);

    if(res == -1) return false;
    return true;
  }

  int exists_node(vector<char> t) {

    // follow labels from root down, edge labels.

    int current_node = store[0].children[t[0]];
 
    if(current_node == -1) return -1;

    //cout << "0navigating to: " << current_node << endl;
    int64_t search_string_position = 0;

    for(;search_string_position < t.size();) {
      // follow edge label
      //cout << "label is: " << store[current_node].label_start << ":" << store[current_node].get_label_end() << endl;
      for(int position=store[current_node].label_start;position <= store[current_node].get_label_end();position++) {
        if(s[position] != t[search_string_position]) { return -1; }
        else {
          //cout << "comparison to position: " << position << endl;
          search_string_position++;
          if(search_string_position == t.size()) { return current_node; }
        }
      }

      current_node = store[current_node].children[t[search_string_position]];
      //cout << "1navigating to: " << current_node << endl;
      if(current_node == -1) return -1;
    }

    //cout << "NOT FOUND" << endl;
    return -1;
  }

  int extend2(int insertion_point,int symbol_index_start,int symbol_index_end,bool &split,int &fnode,int &fpos) {
    //cout << "extend2, insertion_point   : " << insertion_point << endl;
    //cout << "extend2, symbol_index_start: " << symbol_index_start << endl;
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
        SuffixNode sn(insertion_point,symbol_index_start,0);
        sn.label_start = symbol_index_start;
        sn.suffix_link = 0;
        store.push_back(sn);
        split = true;
        store[insertion_point].children[s[symbol_index_start]] = store.size()-1;
        store[store.size()-1].set_depth(store[store[store.size()-1].parent].get_depth());
//cout << "n1" << endl;
//validate_tree();
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
        SuffixNode sn(parent,symbol_index_start,0);
        sn.label_start = symbol_index_start;
        split=true;
        store.push_back(sn);
        store[parent].children[s[symbol_index_start]] = store.size()-1;
        store[store.size()-1].set_depth(store[store[store.size()-1].parent].get_depth());
//cout << "n2" << endl;
        //cout << "2ADD NODE: " << store.size()-1 << endl;
       // cout << "extend2 endpoint 2" << endl;
        return store.size()-1;
      } else {
        // Should really never reach this point.
cout << "NEVER EVER GET HERE EVER EVER" << endl;
dump();
cout << "NEVER EVER GET HERE EVER EVER" << endl;
        exit(0);
      }
    }

//cout << "ZZ9 entry" << endl;
//cout << "ZZ9 len: " << symbol_index_end - symbol_index_start << endl;
//cout << "ZZ9 thing: " << store[insertion_point].get_label_length_r() << endl;

    int start_val =0;
    start_val = symbol_index_end - symbol_index_start; 
    // consume edge label
    if(!dontdoit)
    for(int n=start_val;(n<=edge_length) && (n<=(insert_len));) {
      if(s[symbol_index_start+n] != s[label_start+n]) {
//cout << "ZZ9 split at: " << n << endl;
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

        SuffixNode b(insertion_point,0,0);
        SuffixNode c(insertion_point,0,0);
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
        store[b_idx].set_depth(store[store[b_idx].parent].get_depth()+store[b_idx].get_label_length_r()+1);
        store[c_idx].set_depth(store[store[c_idx].parent].get_depth());
        if(store[insertion_point].label_end == SuffixNode::end_marker) store[insertion_point].set_depth(store[store[insertion_point].parent].get_depth());
        else store[insertion_point].set_depth(store[store[insertion_point].parent].get_depth()+store[insertion_point].get_label_length_r()+1);
	//cout << "n3" << endl;
        //cout << "b_idx: " << b_idx << endl;
        //cout << "c_idx: " << c_idx << endl;
        //cout << "extend2 endpoint 3" << endl;
        return c_idx;
      }

      int a = store[insertion_point].get_label_length_r()+1;
      int b = symbol_index_end-symbol_index_start+1; 

   //   if(a < b) n += a;
   //        else n += b;
//TODO: skip count not present here
      n++;
    }

    // Edge label matched insertion string completely.
    if((edge_length+1) > insert_len) { 
      split=false;

//      if(edge_length == insert_len) split=true; // we matched completely at the last symbol so can be used.
      //cout << "extend2 endpoint 4" << endl;
      return insertion_point;
    }

    //cout << "Extend2 condition 2: checking children" << endl;

    int pos = symbol_index_start + edge_length + 1;

    if(dontdoit) pos--;
    if(pos > symbol_index_end) {
      //cout << "extend2 endpoint 5" << endl;
      //cout << "************************************************************************* WHAT MAENS WHAT MEANS?!?" << endl;
      return insertion_point;
    }

    char child_sym = s[pos];

    // if a child does not exist add
    if(store[insertion_point].children[child_sym] == -1) {
      //cout << "Extend2 condition 2b: no children at point past edge label" << endl;
      //cout << "                      child_sym: " << static_cast<int>(child_sym) << endl;
      SuffixNode newnode(insertion_point,pos,0);
      newnode.label_end   = SuffixNode::end_marker;

      split=true;
      store.push_back(newnode);
      int n_idx = store.size()-1;
      //cout << "***************************************************** 4ADD NODE: " << n_idx << endl;
      store[insertion_point].children[child_sym] = n_idx;
      store[store.size()-1].set_depth(store[store[store.size()-1].parent].get_depth());
	//cout << "n4" << endl;
      //cout << "extend2 endpoint 6" << endl;
      return n_idx;
    }

    // if a child does exist, recurse
    //cout << "recursing, position is now: " << pos << endl;
    return extend2(store[insertion_point].children[child_sym],pos,symbol_index_end,split,fnode,fpos);
  }

  void finalise() {
    insert(final_symbol,true);
  }

  void insert(char current_symbol,bool finalise=false) {

    s.push_back(current_symbol);
    if(!finalise) current_symbol = transcoder.convert(current_symbol);


    SuffixNode::end_marker_value++;

    int last_node=0;
    int last_node_sl=0;
    vector<vector<int64_t> > doall;
    bool nosplitins = true;

    last_node = first_non_leaf_node;
    bool first=true;
    bool split=false;
    int  new_split_count = split_count;

    int predict_node = 0;
    int predict_pos = 0;

    bool first_non_leaf_flag = true;
bool at_end=false;
bool last_at_end=false;
int c=0;
    for(int n=first_non_leaf_n;n<s.size();n++) {
      int  posremin;
      bool insertion;

      last_node_sl = store[last_node].suffix_link;

      int newnode;

      bool last_split=split;
      split=false;

      predict_node = store[last_node].suffix_link;
//      if(first) predict_node = first_node;
      if(first) predict_node = first_non_leaf_node;

      int l_predict_pos = predict_pos;
//      if(first) {/*cout << "pred1" << endl;*/ predict_pos  = (s.size()-1)-store[predict_node].get_label_length_r()+1;} 
      if(store[predict_node].parent       == 0) {/*cout << "pred2" << endl;*/ predict_pos = n;} 

      // Now need to perform 'canonisation' analog.
      int n_label_length = store[last_node]   .get_label_length();
      int s_label_length = store[predict_node].get_label_length();

      int precan_predict_node = predict_node;
      int precan_predict_pos  = predict_pos;

     // lazy canonisation
     //cout << "pre-can predict_node: " << predict_node << endl;
     //cout << "pre-can predict_pos : " << predict_pos << endl;
        predict_pos = n + (store[predict_node].get_depth()-store[predict_node].get_label_length_r())-1;
     if((store[predict_node].parent != 0) && (store[predict_node].suffix_link != 0))
      for(;;) {
//cout << "n is:" << n << endl;
//cout << "cdepth:" << store[predict_node].get_depth() << endl;
        predict_pos = n + (store[predict_node].get_depth()-store[predict_node].get_label_length_r())-1;
//        cout << "precanup: " << predict_pos << endl;
        int ins_size = s.size()-n;
        int c_depth  = store[predict_node].get_depth();
//cout << "ins_size: " << ins_size << endl;
//cout << " c_depth: " << c_depth << endl;
        if(ins_size >= c_depth) break;
        predict_node = store[predict_node].parent;
      }


//      if(first) {cout << "pred1" << endl; predict_pos  = (s.size()-1)-store[predict_node].get_label_length_r()+1;} 

      if(n  == (s.size()-1)) predict_node = 0;
      if(store[predict_node].parent == 0) {/*cout << "pred2a" << endl;*/ predict_node = 0; predict_pos = n;} 

//      cout << "post-can predict_node: " << predict_node << endl;
//      cout << "post-can predict_pos : " << predict_pos << endl;

      int fnode = 0;
      int fpos  = 0;
//      newnode = extend2(0,n,s.size()-1,split,fnode,fpos); // BRUTE
      newnode = extend2(predict_node,predict_pos,s.size()-1,split,fnode,fpos);
//cout << "LOC: " << n << " " << s.size()-1 << endl;
//if(split) cout << "SPLIT TRUE" << endl; else cout << "SPLIT FALSE" << endl;
//cout << "s.size(): " << s.size() << endl;
last_at_end = at_end;
at_end = false;
//int ins_len = s.size()-1-predict_pos;
int ins_len = s.size()-1-n+1;
//int lab_len = store[newnode].get_label_length_r();
int lab_len = store[newnode].get_depth();
if(ins_len == lab_len) at_end = true;
//cout << "ins_len: " << ins_len << endl;
//cout << "lab_len: " << lab_len << endl;
//if(store[newnode].isleaf() == false) cout << "is_leaf: false" << endl;
//if(store[newnode].isleaf() == true ) cout << "is_leaf: true" << endl;


      bool ends_at_magic_leaf = (store[newnode].isleaf() && at_end);
      bool is_the_last_symbol = (n == s.size()-1);
      bool implicit_match     = first_non_leaf_flag && !at_end;

/*      if(at_end            ) cout << "at_end             true" << endl;
      if(ends_at_magic_leaf) cout << "ends_at_magic_leaf true" << endl;
      if(is_the_last_symbol) cout << "is_the_last_symbol true" << endl;
      if(implicit_match    ) cout << "implicit_match     true" << endl;
*/
      if(!first) {

      //if(!is_the_last_symbol && (ends_at_magic_leaf || implicit_match)) {  
      if(!is_the_last_symbol && (ends_at_magic_leaf) && !implicit_match) {  
        // keep going, we don't need to visit these again.
      } else 
      if(first_non_leaf_flag) {
//      if((!ends_at_magic_leaf && implicit_match || (is_the_last_symbol && first_non_leaf_flag))) {
//cout << "will need to revisit, logging" << endl;
//cout << "last_node: " << last_node << endl;
//cout << "n-1: " << n-1 << endl;
        first_non_leaf_node = last_node;
        first_non_leaf_n    = n-1;
       // first_non_leaf_node = newnode;
       // first_non_leaf_n    = n;
        first_non_leaf_flag = false;
      }
}
//first_non_leaf_node = 0;
//first_non_leaf_n = 0;
//      if(first) cout << "************ FFFFIRRRRSSSSSSSTTTTTTT" << endl;
//cout << "fnode: " << fnode << endl;
//cout << "fpos : " << fpos << endl;
//cout << "newnode: " << newnode << endl;
//    if(fnode != predict_node) cout << "********************************************************** NODE PREDICTION FAILURE " << predict_node << "!=" << fnode << endl;
//    if(fpos  != predict_pos ) cout << "**********************************************************  POS PREDICTION FAILURE " << predict_pos  << "!=" << fpos  << endl;

      predict_pos = fpos;

      if(split == true) {
      //  cout << "************************************ split located" << endl;
        new_split_count = 0;
      }

      //if((!first) && split) {
      if((!first) && (split || (at_end && last_at_end && store[newnode].isleaf()))) {
        store[last_node].suffix_link = newnode;
       // store[last_node].suffix_link = newnode;
        //cout << "0SETLINK: " << last_node << " TO " << newnode << endl;
      }

      if((!first) && last_split) {
        store[last_node].suffix_link = newnode;
        store[store[last_node].parent].suffix_link = store[newnode].parent;
        //cout << "1SETLINK: " << store[last_node].parent << " TO " << store[newnode].parent << endl;
      }

/*
      if(first) {
        first_node = newnode;
        //cout << "first_node now: " << first_node << endl;
      }
  */    
      last_node = newnode; // was newnode
//if( split) cout << "SPLIT TRUE" << endl;
//if(!split) cout << "SPLIT FALSE" << endl;
      if((!split) && (c > 0) && !at_end) {
//      if((!first) && (!split)) {
//cout << "BREAK" << endl;
      first=false;
      first_insert=false;
      break;
      }
c++;
      first=false;
      first_insert=false;
    }
/*
    if(split) {
      //cout << "LAST WAS SPLIT: " << store[last_node].parent << endl;
      store[last_node].suffix_link = 0;
      store[store[last_node].parent].suffix_link = 0;
    }
*/
    split_count = new_split_count+1;
  }

  void dump() {
    cout << "****************************** Tree dump" << endl;
    for(int64_t n=0;n<store.size();n++) {
      cout << "node: " << n << endl;
      cout << "label: " << store[n].label_start << " ";
      if(store[n].label_end == SuffixNode::end_marker) cout << store[n].label_end << "(" << SuffixNode::end_marker_value << ")" << endl;
                                                  else cout << store[n].label_end << endl;
      cout << "suffix_link    : " << store[n].suffix_link    << endl;
      cout << "parent         : " << store[n].parent         << endl;
      cout << "depth          : " << store[n].depth          << endl;
      cout << "next_left_leaf : " << store[n].next_left_leaf << endl;
      cout << "next_right_leaf: " << store[n].next_right_leaf << endl;
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

  bool validate_suffix_link(int64_t n,bool dump=false) {
 
   // get my path label
   // get parent chain to root.

    string my_path_label     = get_path_label(n) ;
    string suffix_path_label = get_path_label(store[n].suffix_link);
  
    if(dump) cout << "validating link from/to: " << n << "," << store[n].suffix_link << " labels: " << my_path_label << "," << suffix_path_label << endl;
    if((static_cast<int>(my_path_label.size())-1) > 0)
    if((suffix_path_label.size()) < ((my_path_label.size()-1))) {
      if(dump) cout << "********************************************* SUFFIXLINK DOES NOT VALIDATE, SIZE ERROR1" << endl;
      return false;
    } else
    if((suffix_path_label.size()) != ((my_path_label.size()-1))) {
      if(dump) cout << "********************************************* SUFFIXLINK DOES NOT VALIDATE, SIZE ERROR2" << endl;
      return false;
    } else
    for(int n=0;n<my_path_label.size()-1;n++) {
      if(suffix_path_label[n] != my_path_label[n+1]) {
        if(dump) cout << "****************************************************** SUFFIXLINK DOES NOT VALIDATE" << endl;
        return false;
      }
    }
    return true;
  }

  bool validate_parent(int64_t n,bool dump=false) {
    int64_t parent =  store[n].parent;

    bool ok=false;
    for(int i=0;i<symbol_size;i++) { if(store[parent].children[i] == n) ok = true; }

    if(n == 0) ok = true;
    if(ok != true) {
      if(dump) cout << "************************************************************************************ ERROR PARENT LINK NOT VALIDATED" << endl;
      return false;
    }
    return true;
  }

  bool validate_depth(int n,bool dump=false) {

    bool valid = true;
    int s_depth = store[n].get_depth();

    int depth = 0;
    for(int c=n;store[c].suffix_link != c;) {

      depth += store[c].get_label_length_r()+1;

      c = store[c].parent;
    }

    if(s_depth != depth) {
      if(dump) cout << "ERROR, DEPTH VALIDATION FAILED node = " << n << " calc depth: " << depth << " stored depth: " << s_depth << endl;
      return false;
    }

    return true;
  }

  void dump_stats() {
    cout << "string size: " << s.size() << endl; 
    cout << "node count : " << store.size() << endl;
    size_t end_node = 0;
    for(size_t n=0;n<store.size();n++) if(store[n].label_end == -1) end_node++;
    cout << "end node count: " << end_node << endl;
  }

  bool validate_positions() {
    for(int64_t n=1;n<store.size();n++) {

      int left  = store[n].next_left_leaf;
      int right = store[n].next_right_leaf;

      if((left != -1) && (right != -1) && store[n].isleaf() && store[left].isleaf() && store[right].isleaf()) {
        if(store[left].next_right_leaf != n) { cout << "store[" << left  << "].next_right_leaf=" << store[left].next_right_leaf << "!=" << n << endl; return false;}
        if(store[right].next_left_leaf != n) { cout << "store[" << right << "].next_left_leaf="  << store[left].next_left_leaf  << "!=" << n << endl; return false;}
      }
    }
    return true;
  }

  bool validate_tree(bool dump=false) {
    for(int64_t n=1;n<store.size();n++) {
      bool v1 = validate_depth(n,dump);
      bool v2 = validate_suffix_link(n,dump);
      bool v3 = validate_parent(n,dump);
      if(v1 == false) {cout << "failed depth validation" << endl;       return false;}
      if(v2 == false) {cout << "failed suffix link validation" << endl; return false;}
      if(v3 == false) {cout << "failed parent validation" << endl;      return false;}
    }
    return true;
  }

  TranscodingStore   s;
  //vector<char> s;
  vector<SuffixNode> store;

  Transcode transcoder;

  int root_node;
  int split_point_node;      ///< Point of last insertion/split in tree (node index)
  int split_point_position;  ///< Point of last insertion/split in tree (label position)
  int split_distance;        ///< distance to last split point
  int current_node;          ///< Current node in tree (active point?)
  int posrem;
  bool first_insert;
  int64_t first_node;
  int64_t split_count;
  int first_non_leaf_node;
  int first_non_leaf_n;
};

#endif
