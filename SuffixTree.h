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
#include "ChildStore.h"
#include "global_defs.h"

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
      SuffixNode node = store.get(label_these[n]);
      node.next_left_leaf = label;
      store.set(label_these[n],node);
    }
  }

  bool first_is_leaf(SuffixNode &n) {
    int32_t i = n.get_first_child();
    if(i == -1) return false;

    SuffixNode na = store.get(i);
    return na.is_leaf();
  }

  bool last_is_leaf(SuffixNode &n) {
    int32_t i = n.get_last_child();
    if(i == -1) return false;

    SuffixNode na = store.get(i);
    return na.is_leaf();
  }
 
  void process_positions() {
 //   process_right_positions();
 //   cout << "processed right positions" << endl;
 //   process_left_positions();
 //   cout << "processed left positions" << endl;

    dump();

    int c    = SuffixNode::root;  // start at root vertex.
    int last = -1;

    vector<uint32_t> unlabeled_left;

    bool is_first_child = true;

    uint32_t current_right_most = root_node;
    uint32_t last_right         = root_node;
    for(;;) {

      SuffixNode c_node = store.get(c);

      cout << "c is: " << c << endl;
      cout << "l is: " << last << endl;

      // labeling code

      // left labeling.
      if(c_node.next_left_leaf == -1) unlabeled_left.push_back(c);

      //if(c_node.first_is_leaf()) {
      if(first_is_leaf(c_node)) {
	//cout << "first is leaf" << endl;
	label_all_left_most_down(unlabeled_left,c_node.get_first_child());
	unlabeled_left.clear();
      }

      // right labeling.
      c_node.next_right_leaf = current_right_most;

      //if(c_node.last_is_leaf()) {
      if(last_is_leaf(c_node)) {
	//cout << "last is leaf" << endl;
	current_right_most = c_node.get_last_child();
      }

      // next_right labeling.
      if(c_node.is_leaf()) {
        SuffixNode lr_node = store.get(last_right);
        lr_node.next_right_leaf = c;         /// < serving a different function here.
        store.set(last_right,lr_node);
	last_right = c;
      }

      store.set(c,c_node);

      // walking code
      if(last == -1) {

	last = c;
	int32_t tc = c_node.get_first_child();
	if(tc != -1) {c = tc;} else {
          if(c == root_node) return;
	  if(tc=-1) c = c_node.get_parent();
	  if(c==-1) {return;}
	}
      } else {
	// last != -1
	if(c_node.is_child(last)) {
	  int32_t tc = c_node.next_child(last);
           cout << "next child was identified as: " << tc << endl;
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
    dump();
  }

/*
  void process_right_positions() {

    // 1. in-order traversal

    deque<int> nodestack;
    deque<int> childstack;

    nodestack .push_back(-1);
    childstack.push_back(-1);

    //int alphabet_size = symbol_size-1;
    nodestack .push_back(SuffixNode::root);
    childstack.push_back(alphabet_size);

    int lastleaf = -1;
    for(;;) {

      if(childstack.size() == 0) break;
      if(nodestack.size()  == 0) break;
      int current_node  = nodestack.back();
      nodestack.pop_back();
      int current_child = childstack.back();
      childstack.pop_back(); 

      if(current_node == -1) break;

      SuffixNode current_node_tmp = store.get(current_node);
      int oldl = current_node_tmp.next_right_leaf;
      current_node_tmp.next_left_leaf = lastleaf;
      store.set(current_node,current_node_tmp);

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
          //cout << "current_child: " << current_child << endl;
          if(current_child >= 0) nextchild = current_node_tmp.get_child(current_child);
          //cout << "nextchild    : " << nextchild << endl;

          if(current_child < 0) {
            stop = true;
            nodestack.pop_back();
            childstack.pop_back();
            if(first_pass) {
              current_node_tmp. next_left_leaf  = oldl;
              current_node_tmp. next_right_leaf = lastleaf;
              store.set(current_node,current_node_tmp);
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

    //int alphabet_size = symbol_size-1;
    for(;;) {

      int current_node  = nodestack.back();
      nodestack.pop_back();
      int current_child = childstack.back();
      childstack.pop_back(); 

      if(current_node == -1) break;

      SuffixNode current_node_tmp = store.get(current_node);

      int oldr = current_node_tmp.next_right_leaf;
      current_node_tmp.next_right_leaf = lastleaf;
      store.set(current_node,current_node_tmp);

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

	  int nextchild = current_node_tmp.get_child(current_child);
   
          if(current_child >= alphabet_size) {
            stop = true;
            nodestack.pop_back();
            childstack.pop_back();
            if(first_pass) { 
              current_node_tmp.next_right_leaf = oldr; 
              current_node_tmp.next_left_leaf = lastleaf;
              store.set(current_node,current_node_tmp);
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
*/
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
    SuffixNode current_node_tmp = store.get(current_node);
    for(;search_string_position < ss.size();) {
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
      current_node_tmp = store.get(current_node);
      cout << "current_node: " << current_node << endl;
    }

    return -1;
  }

  string get_substr(uint32_t start,uint32_t end) {
    string res;
cout << "start:" << start << endl;
cout << "end  :" << end << endl;
    if(start > s.size()) return res;
    if(start > end) return res;
    cout << "s.size():" << s.size() << endl;
    if(end > s.size()) end = s.size();
    for(size_t pos=start;pos<=end;pos++) {
      if((pos < s.size()) && (pos >= 0)) res += s.get_uncoded(pos);
     // if((pos < s.size()) && (pos >= 0)) res += s[pos];
    }

    return res;
  }

  vector<int> all_occurs(vector<char> ss,size_t max_hits=-1) {

    vector<int> res;

    for(size_t n=0;n<ss.size();n++) ss[n] =  transcoder.convert(ss[n]);
    int p = find_tree_position(ss);

    if(p == -1) {
      return res;
    }

    // grab left and right...

    SuffixNode p_tmp = store.get(p);
    int nl = p_tmp.next_left_leaf;
    int nr = p_tmp.next_right_leaf;

    if(p_tmp.is_leaf()) {
      res.push_back(s.size()-p_tmp.get_depth());
      return res;
    }

    // itterate from nl until we reach nr

    int c = nl;

    SuffixNode c_tmp = store.get(c);
    bool stop=false;
    for(;stop==false;) {

      if(c==nr) { stop=true; }

      bool nochild=true;
      if(c_tmp.label_start != -1) { res.push_back(s.size()-c_tmp.get_depth()); nochild=false; }//TODO: err somehow convert this back in to correct location?!?

      c = c_tmp.next_right_leaf;
      c_tmp = store.get(c);

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

      SuffixNode current_node_tmp = store.get(current_node);
      for(int position=current_node_tmp.label_start;position <= current_node_tmp.get_label_end();position++) {
        if(s[position] != t[search_string_position]) { return -1; }
        else {
          search_string_position++;
          if(search_string_position == t.size()) { return current_node; }
        }
      }

      current_node = current_node_tmp.get_child(t[search_string_position]);
      if(current_node == -1) return -1;
      current_node_tmp = store.get(current_node);
    }

    return -1;
  }

  int extend2(int insertion_point,int symbol_index_start,int symbol_index_end,bool &split,int &fnode,int &fpos) {
    //cout << "extend2, insertion_point   : " << insertion_point << endl;
    //cout << "extend2, symbol_index_start: " << symbol_index_start << endl;
    //cout << "extend2, label_start       : " << store.get(insertion_point).label_start << endl;
    fnode = insertion_point;
    fpos  = symbol_index_start;

    SuffixNode insertion_point_tmp = store.get(insertion_point);
    int label_start = insertion_point_tmp.label_start;
    int edge_length = insertion_point_tmp.get_label_length();

    int insert_len = symbol_index_end - symbol_index_start;
    // Check edge label

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

        insertion_point_tmp.set_child(s[symbol_index_start],sn_idx);// next insert

        store.set(insertion_point,insertion_point_tmp);
        return sn_idx;
      }
    }

    bool dontdoit=false;
    if(edge_length == 0) {
      // match at label start position?
      if((s[insertion_point_tmp.label_start] == s[symbol_index_start])) {
	symbol_index_start++;
	dontdoit=true;
      } else {
	if((s[insertion_point_tmp.label_start] != s[symbol_index_start])) {
	  // mismatch at label start position - add new child to parent!

	  int parent = insertion_point_tmp.get_parent();
	  SuffixNode parent_tmp = store.get(parent);
	  int child  = parent_tmp.get_child(s[symbol_index_start]);
	  if(child == -1) {
	    // no child here, add one.
	    SuffixNode sn(parent,symbol_index_start,0);
	    sn.label_start = symbol_index_start;
	    split=true;
	    sn.set_depth(parent_tmp.get_depth());
	    int sn_idx = store.push_back(sn);
	    parent_tmp.set_child(s[symbol_index_start],sn_idx);
	    store.set(parent,parent_tmp);
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

        int64_t old_parent        = insertion_point_tmp.parent;
        SuffixNode old_parent_tmp = store.get(insertion_point_tmp.parent);

        int64_t old_label_start = insertion_point_tmp.label_start;

        SuffixNode b(insertion_point,0,0);
        SuffixNode c(insertion_point,0,0);
        b.label_start = old_label_start;
        b.label_end   = old_label_start+n-1;
        c.label_start = symbol_index_start+n;
        c.label_end   = SuffixNode::end_marker;

        insertion_point_tmp.label_start = old_label_start+n;
        store.set(insertion_point,insertion_point_tmp);

        int old_parent_child_symbol = old_parent_tmp.find_child(insertion_point); // TODO: make constant time please?

        old_parent_tmp.set_child(old_parent_child_symbol,b_idx);
        store.set(old_parent,old_parent_tmp);

        //cout << "old_label_start: " << old_label_start << endl;
        //cout << "n              : " << n << endl;
        //cout << "old_label_start+n: " << old_label_start+n << endl;
        b.set_child(s[old_label_start+n],insertion_point);
        b.set_child(s[symbol_index_start+n],c_idx);


        insertion_point_tmp.parent = b_idx;
 //       store.set(insertion_point,insertion_point_tmp);
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
      insertion_point_tmp.set_child(child_sym,n_idx);
      store.set(insertion_point,insertion_point_tmp);

      //extend2 endpoint 6
      return n_idx;

    }

    // if a child does exist, recurse
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

  void insert(char current_symbol,bool finalise=false) {

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

      SuffixNode last_node_tmp = store.get(last_node);
      last_node_sl = last_node_tmp.suffix_link;

      int newnode;

      bool last_split=split;
      split=false;

      predict_node = last_node_tmp.suffix_link;
      if(first) predict_node = first_non_leaf_node;

      SuffixNode predict_node_tmp = store.get(predict_node);
      last_node_tmp    = store.get(last_node); // required?

      //int l_predict_pos = predict_pos;
      if(predict_node_tmp.parent == 0) {/*cout << "pred2" << endl;*/ predict_pos = n;} 

      // Now need to perform 'canonisation' analog.


     // lazy canonisation
     //cout << "pre-can predict_node: " << predict_node << endl;
     //cout << "pre-can predict_pos : " << predict_pos << endl;
     predict_pos = n + (predict_node_tmp.get_depth()-predict_node_tmp.get_label_length_r())-1;
     if((predict_node_tmp.parent != 0) && (predict_node_tmp.suffix_link != 0))
      for(;;) {
        predict_pos = n + (predict_node_tmp.get_depth()-predict_node_tmp.get_label_length_r())-1;
        int ins_size = s.size()-n;
        int c_depth  = predict_node_tmp.get_depth();
        if(ins_size >= c_depth) break;
        predict_node = predict_node_tmp.parent;
        predict_node_tmp = store.get(predict_node);
      }

      if(n  == (s.size()-1)) {predict_node = 0; predict_node_tmp = store.get(0);}
      if(predict_node_tmp.parent == 0) {/*cout << "pred2a" << endl;*/ predict_node = 0; predict_pos = n; predict_node_tmp = store.get(predict_node);} 

//      cout << "post-can predict_node: " << predict_node << endl;
//      cout << "post-can predict_pos : " << predict_pos << endl;

      int fnode = 0;
      int fpos  = 0;
      newnode = extend2(predict_node,predict_pos,s.size()-1,split,fnode,fpos);
      SuffixNode newnode_tmp = store.get(newnode);
      last_node_tmp = store.get(last_node); // required.

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

      if((!first) && (split || (at_end && last_at_end && newnode_tmp.is_leaf()))) {
        if(last_node_tmp.suffix_link != newnode) {  // only perform set if there is a change
          last_node_tmp.suffix_link = newnode;
          store.set(last_node,last_node_tmp);
        }
      }

      if((!first) && last_split) {
        if(last_node_tmp.suffix_link != newnode) {  // only perform set if there is a change
          last_node_tmp.suffix_link = newnode;
          store.set(last_node,last_node_tmp);
        }
        SuffixNode last_node_tmp_parent = store.get(last_node_tmp.parent);
        if(last_node_tmp_parent.suffix_link != newnode_tmp.parent) {  // only perform set if there is a change
          last_node_tmp_parent.suffix_link = newnode_tmp.parent;

          // this is not valid.
          store.set(last_node_tmp.parent,last_node_tmp_parent);
        }
      }

      last_node = newnode; // was newnode
      last_node_tmp = store.get(last_node);
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
  }

  void dump() {
    cout << "****************************** Tree dump" << endl;
    dump_child(0);
    dump(0);
    cout << "****************************** Tree dump complete" << endl;
  }

  void dump(int n) {

    SuffixNode n_tmp = store.get(n);

    for(int i=0;i<symbol_size;i++) {
      if(n_tmp.get_child(i) != -1) { 
        dump_child(n_tmp.get_child(i));
        dump      (n_tmp.get_child(i));
      }
    }
  }


  void dump_child(int n) {
    SuffixNode n_tmp = store.get(n);
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
      SuffixNode parent_tmp = store.get(parent);
      parent = parent_tmp.parent;
    }

    string my_path_label;
    for(int n=parents.size()-1;n>=0;n--) {

      SuffixNode parentsn_tmp = store.get(parents[n]);
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

    SuffixNode n_tmp = store.get(n);
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

    SuffixNode n_tmp = store.get(n);
    string my_path_label     = get_path_label(n) ;
    string suffix_path_label = get_path_label(n_tmp.suffix_link);
  
    if(dump) cout << "validating link from/to: " << n << "," << n_tmp.suffix_link << " labels: " << my_path_label << "," << suffix_path_label << endl;
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

    SuffixNode n_tmp = store.get(n);
    int64_t parent =  n_tmp.parent;

    SuffixNode parent_tmp = store.get(parent);
    bool ok=false;
    for(int i=0;i<symbol_size;i++) { if(parent_tmp.get_child(i) == n) ok = true; }

    if(n == 0) ok = true;
    if(ok != true) {
      if(dump) cout << "************************************************************************************ ERROR PARENT LINK NOT VALIDATED: " << n << endl;
      return false;
    }
    return true;
  }

  bool validate_depth(int n,bool dump=false) {

    //bool valid = true;
    SuffixNode n_tmp = store.get(n);
    int s_depth = n_tmp.get_depth();

    int depth = 0;

    SuffixNode c_tmp = store.get(n);
    for(int c=n;c_tmp.suffix_link != c;) {

      depth += c_tmp.get_label_length_r()+1;

      c = c_tmp.parent;
      c_tmp = store.get(c);
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
    cout << "Size of ChildStore pointer    : " << sizeof(ChildStore *) << endl;
  }

  bool validate_positions() {
    for(int64_t n=1;n!=store.last_idx();n=store.next_idx(n)) {

      SuffixNode n_tmp = store.get(n);
      int left  = n_tmp.next_left_leaf;
      int right = n_tmp.next_right_leaf;

      SuffixNode left_tmp = store.get(left);
      SuffixNode right_tmp = store.get(right);
      if((left != -1) && (right != -1) && n_tmp.is_leaf() && left_tmp.is_leaf() && right_tmp.is_leaf()) {
        if(left_tmp.next_right_leaf != n) { cout << "store[" << left  << "].next_right_leaf=" << left_tmp.next_right_leaf << "!=" << n << endl; return false;}
        if(right_tmp.next_left_leaf != n) { cout << "store[" << right << "].next_left_leaf="  << left_tmp.next_left_leaf  << "!=" << n << endl; return false;}
      }
    }
    return true;
  }

  bool validate_tree(bool dump=false) {
    for(int64_t n=1;n!=store.last_idx();n=store.next_idx(n)) {
      bool v1 = validate_depth(n,dump);
      bool v2 = validate_suffix_link(n,dump);
      bool v3 = validate_parent(n,dump);
      if(v1 == false) {cout << "failed depth validation" << endl;       return false;}
      if(v2 == false) {cout << "failed suffix link validation" << endl; return false;}
      if(v3 == false) {cout << "failed parent validation" << endl;      return false;}
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
