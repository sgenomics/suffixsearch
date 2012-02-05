#ifndef SUFFIXTREE
#define SUFFIXTREE

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <stdlib.h>
#include "SearchStore.h"
#include "Transcode.h"
#include "TranscodingStore.h"
#include "SuffixNodeStore.h"
#include "SuffixNodeStoreMemVec.h"
#include "SuffixNode.h"
#include "global_defs.h"

#define suffixnode_t SuffixNode&

using namespace std;

class SuffixTree {

public:
  SuffixTree() {
    
    SuffixNode root(0,-1,-1);
    root.suffix_link = 0;
    root.parent = 0;
    split_count = 0;
    root_node = store.push_back(root);

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

  void label_all_left_most_down(vector<uint32_t> &label_these,int32_t label) {
    for(size_t n=0;n<label_these.size();n++) {

      suffixnode_t node = store.get(label_these[n]);
      node.next_left_leaf = label;
      store.set(label_these[n],node);
    }
  }

  bool first_is_leaf(SuffixNode &n) {
    int32_t i = n.get_first_child();
    if(i == -1) return false;

    suffixnode_t na = store.get(i);
    return na.is_leaf();
  }

  bool last_is_leaf(SuffixNode &n) {
    int32_t i = n.get_last_child();
    if(i == -1) return false;

    suffixnode_t na = store.get(i);
    return na.is_leaf();
  }
 
  void process_positions() {
    cout << "Processing positions" << endl;

    int c    = SuffixNode::root;  // start at root vertex.
    int last = -1;

    vector<uint32_t> unlabeled_left;

    bool is_first_child = true;

    uint32_t current_right_most = root_node;
    uint32_t last_right         = root_node;
    for(;;) {
      //SuffixNode c_node = store.get(c);
      suffixnode_t c_node = store.get(c);

      // labeling code

      // left labeling.
      if(c_node.next_left_leaf == -1) unlabeled_left.push_back(c);

      if(first_is_leaf(c_node)) {
	label_all_left_most_down(unlabeled_left,c_node.get_first_child());
	unlabeled_left.clear();
        // c_node = store.get(c); // WHY WAS THIS REQUIRED?
      }

      // right labeling.
      c_node.next_right_leaf = current_right_most;

      //if(c_node.last_is_leaf()) {
      if(last_is_leaf(c_node)) {
	current_right_most = c_node.get_last_child();
      }

      // next_right labeling.
      if(c_node.is_leaf()) {
        suffixnode_t lr_node = store.get(last_right);
        lr_node.next_right_leaf = c;         /// < serving a different function here.
        store.set(last_right,lr_node);
	last_right = c;
      }

 //     SuffixNode &cc_node = store.get(c);
//      cc_node = c_node;
      store.set(c,c_node);

      // walking code
      if(last == -1) {

	last = c;
	int32_t tc = c_node.get_first_child();
	if(tc != -1) {c = tc;} else {
          if(c == root_node) return;
	  if(tc==-1) c = c_node.get_parent();
	  if(c==-1) {return;}
	}
      } else {
	// last != -1
	if(c_node.is_child(last)) {
	  int32_t tc = c_node.next_child(last);
	  last = -1;// MOVED THIS AROUND.
	  if(tc!=-1) { c = tc; }
	  else       { if(c==root_node) return; last = c; c = c_node.get_parent(); if(c == -1) {return;} }
	} else {
	  // is_child(last,c,vertexes) == false

	  last=c;
	  int tc = c_node.get_first_child();
	  if(tc!=-1) {c =tc;}
	  else {
            if(c == root_node) return;
	    c = c_node.get_parent();
	    if(c==-1) {return;}
	  }
	}
      }
    }
  }

  int find_tree_position(vector<char> ss) {
    // follow labels from root down, edge labels.

    int current_node = store.get(SuffixNode::root).get_child(ss[0]);

    char label;
    if(ss.size() == 1) {
      label = 0;
    } else {
      label = ss[1];
    }
 
    size_t search_string_position = 0;
    if(current_node == -1) return -1;
    for(;search_string_position < ss.size();) {
      suffixnode_t current_node_tmp = store.get(current_node);
      // follow edge label
      for(int position=current_node_tmp.label_start;position <= current_node_tmp.get_label_end();position++) {
        if(s[position] != ss[search_string_position]) {return -1;}
        else {
          search_string_position++;
          if(search_string_position == ss.size()) {
            return current_node;
          }
        }
      }

      label = ss[search_string_position];

      current_node = current_node_tmp.get_child(label);
      if(current_node == -1) return -1;
      // current_node_tmp = store.get(current_node);
    }

    return -1;
  }

  string get_substr(uint32_t start,uint32_t end) {
    string res;
    if(start > s.size()) return res;
    if(start > end) return res;
    if(end > s.size()) end = s.size();
    for(size_t pos=start;pos<=end;pos++) {
      if((pos < s.size()) && (pos >= 0)) res += s.get_uncoded(pos);
     // if((pos < s.size()) && (pos >= 0)) res += s[pos];
    }

    return res;
  }

  vector<int> all_occurs(vector<char> ss,size_t max_hits=-1) {
    cout << "find all occurs..." << endl;
    vector<int> res;

    for(size_t n=0;n<ss.size();n++) ss[n] =  transcoder.convert(ss[n]);
    int p = find_tree_position(ss);

    if(p == -1) {
      return res;
    }

    // grab left and right...

    suffixnode_t p_tmp = store.get(p);
    int nl = p_tmp.next_left_leaf;
    int nr = p_tmp.next_right_leaf;

    if(p_tmp.is_leaf()) {
      res.push_back(s.size()-p_tmp.get_depth());
      return res;
    }

    // itterate from nl until we reach nr

    int c = nl;

    bool stop=false;
    for(;stop==false;) {

      suffixnode_t c_tmp = store.get(c);
      if(c==nr) { stop=true; }

      bool nochild=true;
      if(c_tmp.label_start != -1) { res.push_back(s.size()-c_tmp.get_depth()); nochild=false; }//TODO: err somehow convert this back in to correct location?!?

      c = c_tmp.next_right_leaf;
    //  c_tmp = store.get(c);

      if(res.size() > max_hits) return res;
    }

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

    int current_node = store.get(SuffixNode::root).get_child(t[0]);
 
    if(current_node == -1) return -1;

    uint64_t search_string_position = 0;

    for(;search_string_position < t.size();) {
      // follow edge label

      suffixnode_t current_node_tmp = store.get(current_node);
      for(int position=current_node_tmp.label_start;position <= current_node_tmp.get_label_end();position++) {
        if(s[position] != t[search_string_position]) { return -1; }
        else {
          search_string_position++;
          if(search_string_position == t.size()) { return current_node; }
        }
      }

      current_node = current_node_tmp.get_child(t[search_string_position]);
      if(current_node == -1) return -1;
    //  current_node_tmp = store.get(current_node);
    }

    return -1;
  }

  int extend2(int insertion_point,int symbol_index_start,int symbol_index_end,bool &split,int &fnode,int &fpos) {
    fnode = insertion_point;
    fpos  = symbol_index_start;

    suffixnode_t insertion_point_tmp0 = store.get(insertion_point);
    int label_start = insertion_point_tmp0.label_start;
    int edge_length = insertion_point_tmp0.get_label_length();

    int insert_len = symbol_index_end - symbol_index_start;
    // Check edge label
    {
      suffixnode_t insertion_point_tmp = store.get(insertion_point);
      // this means we're at the root node, it's kind of special!
      if(insertion_point_tmp.label_start == -1) {

        // if a child exists, go to it, without consuming
        int child = insertion_point_tmp.get_child(s[symbol_index_start]);
        if(child != -1) {
          return extend2(child,symbol_index_start,symbol_index_end,split,fnode,fpos);
        } else {
          // if it doesn't exist add it.
          SuffixNode sn(insertion_point,symbol_index_start,0);
          sn.label_start = symbol_index_start;
          sn.label_end   = -1;
          sn.suffix_link = 0;
          split = true;
          sn.set_depth(insertion_point_tmp.get_depth());

          int sn_idx = store.push_back(sn);
          suffixnode_t insertion_point_tmp1 = store.get(insertion_point);
          insertion_point_tmp1.set_child(s[symbol_index_start],sn_idx);// next insert

          store.set(insertion_point,insertion_point_tmp1);
          return sn_idx;
        }
      }
    }

    bool dontdoit=false;
    if(edge_length == 0) {
      suffixnode_t insertion_point_tmp = store.get(insertion_point);
      // match at label start position?
      if((s[insertion_point_tmp.label_start] == s[symbol_index_start])) {
	symbol_index_start++;
	dontdoit=true;
      } else {
	if((s[insertion_point_tmp.label_start] != s[symbol_index_start])) {
	  // mismatch at label start position - add new child to parent!

	  int parent = insertion_point_tmp.get_parent();
	  suffixnode_t parent_tmp = store.get(parent);
	  int child  = parent_tmp.get_child(s[symbol_index_start]);
	  if(child == -1) {
	    // no child here, add one.
	    SuffixNode sn(parent,symbol_index_start,0);
	    sn.label_start = symbol_index_start;
	    split=true;
	    sn.set_depth(parent_tmp.get_depth());
	    int sn_idx = store.push_back(sn);

	    suffixnode_t parent_tmp1 = store.get(parent);
	    parent_tmp1.set_child(s[symbol_index_start],sn_idx);
	    store.set(parent,parent_tmp1);
	    return sn_idx;
	  } else {
	    return extend2(child,symbol_index_start+1,symbol_index_end,split,fnode,fpos);
	  }
	}
      }
    }

    int start_val =0;
    start_val = symbol_index_end - symbol_index_start; 
    // consume edge label
    if(!dontdoit)
    for(int n=start_val;(n<=edge_length) && (n<=(insert_len));) {
      if(s[symbol_index_start+n] != s[label_start+n]) {
        // mismatch on edge label
        //Extend2 condition 1: Mismatch in edge label

        int b_idx = store.push_back_norm();
        int c_idx = store.push_back_end();

        suffixnode_t insertion_point_tmp = store.get(insertion_point);
        int64_t old_parent        = insertion_point_tmp.parent;
        suffixnode_t old_parent_tmp = store.get(insertion_point_tmp.parent);

        int64_t old_label_start = insertion_point_tmp.label_start;

        suffixnode_t b = store.get(b_idx); //b(insertion_point,0,0);
        suffixnode_t c = store.get(c_idx); //c(insertion_point,0,0);
        b.label_start = old_label_start;
        b.label_end   = old_label_start+n-1;
        c.label_start = symbol_index_start+n;
        c.label_end   = SuffixNode::end_marker;

        insertion_point_tmp.label_start = old_label_start+n;
        store.set(insertion_point,insertion_point_tmp); // WHY IS THIS REQUIRED HERE?

        int old_parent_child_symbol = old_parent_tmp.find_child(insertion_point); // TODO: make constant time please?

        old_parent_tmp.set_child(old_parent_child_symbol,b_idx);
        store.set(old_parent,old_parent_tmp);

        b.set_child(s[old_label_start+n],insertion_point);
        b.set_child(s[symbol_index_start+n],c_idx);

        insertion_point_tmp.parent = b_idx;
        c.parent = b_idx;
        b.parent = old_parent;
        b.suffix_link = 0;// (this is pointed after the next insertion in insert)
        c.suffix_link = 0;

        // ADD NODE: b_idx
        // ADD NODE: c_idx 
        split=true;
        b.set_depth(old_parent_tmp.get_depth()+b.get_label_length_r()+1);
        c.set_depth(b.get_depth());
        store.set(b_idx,b);
        store.set(c_idx,c);

        if(insertion_point_tmp.label_end == SuffixNode::end_marker) { insertion_point_tmp.set_depth(b.get_depth());}
        else                                                        { insertion_point_tmp.set_depth(b.get_depth()+insertion_point_tmp.get_label_length_r()+1);}
        store.set(insertion_point,insertion_point_tmp);
        //extend2 endpoint 3
        return c_idx;
      }

      n++;
    }

    // Edge label matched insertion string completely.
    if((edge_length+1) > insert_len) { 
      split=false;

      return insertion_point;
    }

    //Extend2 condition 2: checking children
    int pos = symbol_index_start + edge_length + 1;

    if(dontdoit) pos--;
    if(pos > symbol_index_end) {
      //extend2 endpoint 5: broken?
      return insertion_point;
    }

    char child_sym = s[pos];

    {
      suffixnode_t insertion_point_tmp = store.get(insertion_point);
      if(insertion_point_tmp.get_child(child_sym) == -1) {

	// we seem to get here even though insertion point has a label length of 0... how?
	// we should replace the edge label with our own...

	if(insertion_point_tmp.label_end == -1) {
	  store.set(insertion_point,insertion_point_tmp);
	  return insertion_point;
	}

	int l = insertion_point_tmp.get_label_length_r();
   
	if(insertion_point_tmp.is_leaf()) {
	  cout << "WTF it's a leaf?!?" << endl;
	  exit(0);
	}

	//Extend2 condition 2b: no children at point past edge label

	// if we get here we really MUST have other children.
	SuffixNode newnode(insertion_point,pos,0);
	newnode.label_end   = SuffixNode::end_marker;

	split=true;
	newnode.set_depth(insertion_point_tmp.get_depth());
	int n_idx = store.push_back(newnode);

	//ADD NODE: n_idx
	suffixnode_t insertion_point_tmp1 = store.get(insertion_point);
	insertion_point_tmp1.set_child(child_sym,n_idx);
	store.set(insertion_point,insertion_point_tmp1);

	//extend2 endpoint 6
	return n_idx;

      }
    }

    // if a child does exist, recurse

    suffixnode_t insertion_point_tmp = store.get(insertion_point);
    return extend2(insertion_point_tmp.get_child(child_sym),pos,symbol_index_end,split,fnode,fpos);
  }

  void finalise() {
    insert(39,true);
  }

  void insert(string insertion_string) {
    for(size_t n=0;n<insertion_string.size();n++) {
      insert(insertion_string[n]);
    }
  }

  void insert(unsigned char current_symbol,bool finalise=false) {

    s.push_back(current_symbol);
    if(!finalise) current_symbol = transcoder.convert(current_symbol);

    

    SuffixNode::end_marker_value++;

    int last_node=0;
    int last_node_sl=0;
    vector<vector<int64_t> > doall;

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
    for(size_t n=first_non_leaf_n;n<s.size();n++) {

      suffixnode_t last_node_tmp1 = store.get(last_node);
      last_node_sl = last_node_tmp1.suffix_link;

      int newnode;

      bool last_split=split;
      split=false;

      predict_node = last_node_tmp1.suffix_link;
      if(first) predict_node = first_non_leaf_node;

      suffixnode_t predict_node_t1 = store.get(predict_node);
 //     last_node_tmp    = store.get(last_node); // required?

      //int l_predict_pos = predict_pos;
      if(predict_node_t1.parent == 0) { predict_pos = n;} 

      // Now need to perform 'canonisation' analog.


     // lazy canonisation
     predict_pos = n + (predict_node_t1.get_depth()-predict_node_t1.get_label_length_r())-1;
     if((predict_node_t1.parent != 0) && (predict_node_t1.suffix_link != 0))
      for(;;) {
        suffixnode_t predict_node_c = store.get(predict_node);

        predict_pos = n + (predict_node_c.get_depth()-predict_node_c.get_label_length_r())-1;
        int ins_size = s.size()-n;
        int c_depth  = predict_node_c.get_depth();
        if(ins_size >= c_depth) break;
        predict_node = predict_node_c.parent;
 //       predict_node_tmp = store.get(predict_node);
      }

      if(n  == (s.size()-1)) {predict_node = 0;}

      suffixnode_t predict_node_t2 = store.get(predict_node);

      if(predict_node_t2.parent == 0) {predict_node = 0; predict_pos = n;} 

      int fnode = 0;
      int fpos  = 0;
      newnode = extend2(predict_node,predict_pos,s.size()-1,split,fnode,fpos);
      suffixnode_t newnode_tmp = store.get(newnode);
      last_at_end = at_end;
      at_end = false;
      int ins_len = s.size()-1-n+1;
      int lab_len = newnode_tmp.get_depth();
      if(ins_len == lab_len) at_end = true;


      bool ends_at_magic_leaf = (newnode_tmp.is_leaf() && at_end);
      bool is_the_last_symbol = (n == s.size()-1);
      bool implicit_match     = first_non_leaf_flag && !at_end;

      if(!first) {

        if(!is_the_last_symbol && (ends_at_magic_leaf) && !implicit_match) {  
          // keep going, we don't need to visit these again.
        } else 
        if(first_non_leaf_flag) {
          first_non_leaf_node = last_node;
          first_non_leaf_n    = n-1;
          first_non_leaf_flag = false;
        }
      }

      predict_pos = fpos;

      if(split == true) {
        new_split_count = 0;
      }

      suffixnode_t last_node_tmp2 = store.get(last_node); // required.
      if((!first) && (split || (at_end && last_at_end && newnode_tmp.is_leaf()))) {
        if(last_node_tmp2.suffix_link != newnode) {  // only perform set if there is a change
          last_node_tmp2.suffix_link = newnode;
          store.set(last_node,last_node_tmp2);
        }
      }

      if((!first) && last_split) {
        if(last_node_tmp2.suffix_link != newnode) {  // only perform set if there is a change
          last_node_tmp2.suffix_link = newnode;
          store.set(last_node,last_node_tmp2);
        }
        suffixnode_t last_node_tmp_parent = store.get(last_node_tmp2.parent);
        if(last_node_tmp_parent.suffix_link != newnode_tmp.parent) {  // only perform set if there is a change
          last_node_tmp_parent.suffix_link = newnode_tmp.parent;

          store.set(last_node_tmp2.parent,last_node_tmp_parent);
        }
      }

      last_node = newnode; // was newnode
 ////////NOTREQUIRED?!!!!     last_node_tmp = store.get(last_node);
      if((!split) && (c > 0) && !at_end) {
        first=false;
        first_insert=false;
        break;
      }
      c++;

      first=false;
      first_insert=false;
    }

    split_count = new_split_count+1;
    //validate_tree();
  }

  void dump() {
    cout << "****************************** Tree dump" << endl;
    dump_child(0);
    dump(0);
    cout << "****************************** Tree dump complete" << endl;
  }

  void dump(int n) {

    suffixnode_t n_tmp = store.get(n);

    for(int i=0;i<symbol_size;i++) {
      if(n_tmp.get_child(i) != -1) { 
        dump_child(n_tmp.get_child(i));
        dump      (n_tmp.get_child(i));
      }
    }
  }


  void dump_child(int n) {
    suffixnode_t n_tmp = store.get(n);
    cout << "node: " << n << endl;
    cout << "label: " << n_tmp.label_start << " ";
    if(n_tmp.label_end == SuffixNode::end_marker) cout << n_tmp.label_end << "(" << SuffixNode::end_marker_value << ")" << endl;
                                                  else cout << n_tmp.label_end << endl;
    cout << "suffix_link    : " << n_tmp.suffix_link    << endl;
    cout << "parent         : " << n_tmp.parent         << endl;
    cout << "depth          : " << n_tmp.depth          << endl;
    cout << "next_left_leaf : " << n_tmp.next_left_leaf << endl;
    cout << "next_right_leaf: " << n_tmp.next_right_leaf << endl;

    bool has_child=false;
    for(int i=0;i<symbol_size;i++) {
      if(n_tmp.get_child(i) != -1) {
        cout << "children[" << i << "]:" << n_tmp.get_child(i) << endl; 
        has_child=true;
      }
    }

    if(has_child==false) {
      cout << "************** no children" << endl;
    }
  }

  string get_path_label(int n) {

    if(n==0) return "";

    vector<int64_t> parents;
    int64_t parent = n;
    for(;parent != 0;) {
      parents.push_back(parent);
      suffixnode_t parent_tmp = store.get(parent);
      parent = parent_tmp.parent;
    }

    string my_path_label;
    for(int n=parents.size()-1;n>=0;n--) {

      suffixnode_t parentsn_tmp = store.get(parents[n]);
      int start = parentsn_tmp.label_start;
      int end   = parentsn_tmp.get_label_end();
      for(int i=start;i<=end;i++) {
        my_path_label += s[i];
      }
    }
    return my_path_label;
  }

  string get_label(int n) {
    if(n==0) return "";

    suffixnode_t n_tmp = store.get(n);
    int start = n_tmp.label_start;
    int end   = n_tmp.get_label_end();
    string my_path_label;
    for(int i=start;i<=end;i++) {
      my_path_label += s[i];
    }
    return my_path_label;
  }

  bool validate_suffix_link(int64_t n,bool dump=false) {
 
   // get my path label
   // get parent chain to root.

    suffixnode_t n_tmp = store.get(n);
    string my_path_label     = get_path_label(n) ;
    string suffix_path_label = get_path_label(n_tmp.suffix_link);
  
    if(dump) cout << "validating link from/to: " << n << "," << (int) n_tmp.suffix_link << " label lens: " << my_path_label.size() << "," << suffix_path_label.size() << endl;
    if((static_cast<int>(my_path_label.size())-1) > 0) {
      if((suffix_path_label.size()) < ((my_path_label.size()-1))) {
	if(dump) cout << "********************************************* SUFFIXLINK DOES NOT VALIDATE, SIZE ERROR1" << endl;
	return false;
      } else
      if((suffix_path_label.size()) != ((my_path_label.size()-1))) {
	if(dump) cout << "********************************************* SUFFIXLINK DOES NOT VALIDATE, SIZE ERROR2" << endl;
	return false;
      } else {
	for(size_t n=0;n<my_path_label.size()-1;n++) {
	  if(suffix_path_label[n] != my_path_label[n+1]) {
	    if(dump) cout << "****************************************************** SUFFIXLINK DOES NOT VALIDATE" << endl;
	    return false;
	  }
	}
      }
    }
    return true;
  }

  bool validate_parent(int64_t n,bool dump=false) {

    suffixnode_t n_tmp = store.get(n);
    int64_t parent =  n_tmp.parent;

    suffixnode_t parent_tmp = store.get(parent);
    bool ok=false;
    for(int i=0;i<symbol_size;i++) { if(parent_tmp.get_child(i) == n) ok = true; }

    if(n == 0) ok = true;
    if(ok != true) {
      if(dump) cout << "******************************** ERROR PARENT LINK NOT VALIDATED: node " << n << " stored parent: " << parent << " but this node not child" << endl;
      return false;
    }
    return true;
  }

  bool validate_depth(int n,bool dump=false) {

    //bool valid = true;
    suffixnode_t n_tmp = store.get(n);
    int s_depth = n_tmp.get_depth();

    int depth = 0;

    bool fin=false;

    if(n_tmp.suffix_link == n) fin = true;
    for(int c=n;fin==false;) {
      suffixnode_t c_tmp = store.get(c);
      if(c_tmp.suffix_link == c) fin = true;
      else {

        depth += c_tmp.get_label_length_r()+1;

        c = c_tmp.parent;
      }
    }

    if(s_depth != depth) {
      if(dump) cout << "ERROR, DEPTH VALIDATION FAILED node = " << n << " calc depth: " << depth << " stored depth: " << s_depth << endl;
      return false;
    }

    return true;
  }

  size_t size() {
    return s.size();
  }

  void dump_stats() {
    cout << "string size: " << s.size() << endl; 
    cout << "node count : " << store.size() << endl;
    // size_t end_node = 0;

    store.stats();

    cout << "Unpopulated SuffixNode size is: " << sizeof(SuffixNode) << endl;
    cout << "Unpopulated SuffixTree size is: " << sizeof(SuffixTree) << endl;
  }

  bool validate_positions() {
/*
REQUIRES REWRITE
    for(int64_t n=1;n!=store.last_idx();n=store.next_idx(n)) {

      suffixnode_t n_tmp = store.get(n);
      int left  = n_tmp.next_left_leaf;
      int right = n_tmp.next_right_leaf;

      suffixnode_t left_tmp = store.get(left);
      suffixnode_t right_tmp = store.get(right);
      if((left != -1) && (right != -1) && n_tmp.is_leaf() && left_tmp.is_leaf() && right_tmp.is_leaf()) {
        if(left_tmp.next_right_leaf != n) { cout << "store[" << left  << "].next_right_leaf=" << left_tmp.next_right_leaf << "!=" << n << endl; return false;}
        if(right_tmp.next_left_leaf != n) { cout << "store[" << right << "].next_left_leaf="  << left_tmp.next_left_leaf  << "!=" << n << endl; return false;}
      }
    }
*/
    return true;
  }

  bool validate_tree(bool dump=false) {
    for(int64_t n=1;n!=store.last_idx();n=store.next_idx(n)) {
      if(dump) cout << "validating: " << n << endl;
      //bool v1 = validate_depth(n,dump);
      //bool v2 = validate_suffix_link(n,dump);
      bool v3 = validate_parent(n,dump);
      //if(v1 == false) {cout << "failed depth validation" << endl;       }
      //if(v2 == false) {cout << "failed suffix link validation" << endl; }
      if(v3 == false) {cout << "failed parent validation" << endl;      }

      //if(v1 == false) return false;
      //if(v2 == false) return false;
      if(v3 == false) return false;
    }
    return true;
  }

  void compact() {
    store.force_compact();
  }

  void set_compactmode(bool compactmode) {
    store.set_compactmode(compactmode);
  }

  TranscodingStore     s;
  suffixnodestore_type store;

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
