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

    return 0;
  }

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
    SuffixNode::end_marker_value = 0;
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
    return false;
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

    for(int n=s.size()-1;n>=0;n--) {
      int  posremin;
      bool insertion;
      cout << "******************************************** position: " << n << endl;
      int newnode = extend(0,current_symbol,s.size()-1,insertion,n,false,posremin);
      dump();
    }
    SuffixNode::end_marker_value++;

/*
    bool insertion=false;
    cout << "split_distance: " << split_distance << endl;

    int position = split_distance; 


    cout << "preloop" << endl;
    cout << "position is: " << position << endl;
    cout << "split_point_node: " << split_point_node << endl;
    cout << "POSREM: " << posrem << endl;
    cout << "split_distance: " << split_distance << endl;
  /////////////////////////////////////  current_node = split_point_node;
    current_node = split_point_node;
    if((posrem==1)  && (store[split_point_node].edge_label_length() == 0)) {
      cout << "USING POSREM, origianl node: " << split_point_node << endl;
      cout << "pos-1 is: " << s[s.size()-2] << endl;
      cout << "label+1 is: " << s[store[split_point_node].label_start+1] << endl;
//      current_node = store[split_point_node].children[   s[    s.size()-2     ]         ];
//      current_node = store[split_point_node].children[   s[ store[split_point_node].label_start+1     ]         ];
      cout << "USING POSREM, new node: " << current_node << endl;
      position = position;//+posrem;//?!?
    }
    cout << "BETA position      : " << position << endl;

    bool any_insert=false;
    bool first_insertion=true;

    for(size_t n=0;n<store.size();n++) store[n].olink = -1; // AAAAAAAAAAAARRRRGGGHHHH NON LINEAR!

    bool firsta=true;
    position = store[current_node].edge_label_length()-1;
    for(;position>=0;) {

      int link;
      int link_off;

    if(store[current_node].olink == -1) { store[current_node].olink = store[current_node].suffix_link; store[current_node].olink_off = store[current_node].suffix_offset; }
   // link     = store[current_node].olink; 
    if(store[link].olink == -1) { store[link].olink = store[link].suffix_link; store[link].olink_off = store[link].suffix_offset; }

     if(!firsta)     link     = store[current_node].olink; else link=current_node;
 firsta=false;
    //  link = store[current_node].suffix_link; 
      cout << "split_distance             : " << split_distance << endl;
      cout << "label                      : " << store[link].label_start << endl;

      cout << "insaneoflex position: " << position << endl;

      int newnode;

      cout << "*** insertion: link: " << link << " current_symbol: " << current_symbol << " position: " << position << " insertion: " << insertion << " split_dist: " << split_distance << endl;
      int posremin=0;
      newnode = extend(link,current_symbol,s.size()-1,insertion,position,false,posremin);
//      newnode = extend(link,current_symbol,s.size()-1,insertion,position+link_off_r,false,posremin);
//      store[link].suffix_link = newnode;///////////////////////////////////////////////////////////
      store[current_node].suffix_link = newnode;///////////////////////////////////////////////////////////
      cout << "GAMMA currentnode: " << current_node << endl;
      cout << "GAMMA link       : " << link << endl;
      cout << "GAMMA newnode    : " << newnode << endl;

///////////  if(first) position-=posrem;
//      if(posrem == 0) store[current_node].suffix_offset = -1;
//                 else store[current_node].suffix_offset = s.size()-1-posrem;//link_off;//store[current_node].suffix_offset; // SOMETHINGS ELESES

      current_node = link; // newnode; // something...

      cout << "new current_node is : " << current_node << endl;

      if(!insertion) {
         cout << "************************************************** insertion was false, incremented split_distance" << endl;
      }

      position--;
 /////     current_node = oldlink;
      if(insertion && first_insertion) {
        cout << "DELTA UPDATING SPLIT LOCATION" << endl;
        cout << "position was: " << position+1 << endl;
        cout << "posrem: " << posremin << endl;
        cout << "splitnode: " << newnode << endl;
        split_point_node=newnode; //current_node;
//position-=posrem;//BORKTOWN
        posrem=posremin;
        split_point_position=s.size()-1;
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
*/
  }


  int extend(int insertion_point,int symbol,int symbol_index,bool &insertion,int position,bool test,int &posrem) {

    cout << "performing extension" << endl;

    cout << "insertion p  : " << insertion_point << endl;
    cout << "label length : " << store[insertion_point].edge_label_length() << endl;
    cout << "position     : " << position << endl;

    posrem = 0;

    if(position < store[insertion_point].edge_label_length()) {

      char match_point_symbol = s[store[insertion_point].label_start+position];

      cout << "match point symbol is: " << match_point_symbol << endl;
      if(match_point_symbol == symbol) { cout << "extend condition -1"; return insertion_point; }

      // insertion can occur at this point
      cout << "extend condition 0" << endl;
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
      n2.suffix_link = 0;
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
      n3.suffix_link = 0;
      n3.suffix_offset = symbol_index;
      store.push_back(n3);
      int n3_idx = store.size()-1;

      store[insertion_point].clear_children();
      store[insertion_point].children[mismatch_symbol] = n2_idx;
      store[insertion_point].children[symbol] = n3_idx;
    } else {
      cout << "extend condition 1" << endl;

      if(store[insertion_point].children[s[symbol_index-position]] == -1) {
        cout << "extend condition 2" << endl;
        int startpoint = symbol_index-position;
        char sym = s[startpoint];
        SuffixNode n(insertion_point,startpoint);
        store.push_back(n);
        store[insertion_point].children[sym] = store.size()-1;
        return store.size()-1;
      }

      int labellen = store[insertion_point].edge_label_length();
      return extend(store[insertion_point].children[s[symbol_index-position]],s[symbol_index-position],symbol_index-position,insertion,position-labellen,test,posrem);
    }

/*
    if(insertion_point == 0) {
      cout << "extend condition -0a, symbol_index is: " << symbol_index << endl;
      cout << "extend condition -0a, position is: " << position << endl;
      cout << "extend condition -0a" << endl;
      insertion_point = store[0].children[symbol];//bork change
      //insertion_point = store[0].children[s[symbol_index-position]];//+1
//      insertion_point = store[0].children[s[symbol_index-position+1]];//+1
      cout << "extend condition -0a insertion_point now: " << insertion_point << endl;
      if(insertion_point == -1) {

       //  if(position == 0) position = 1;
         cout << "extend condition -0b 0 " << endl;
         // there is no child with this symbol, add one!
         SuffixNode newnode(0,(symbol_index-position)); // -position (make this generalise was +1
 //        newnode.suffix_offset = symbol_index; // THIS IS WHAT I DONE ADDED
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

         return nidx;// here be dragoons BETABORK
         return 0;// here be dragoons
///         return nidx;
      }
    }
*/

/*
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
       if(store[insertion_point].children[symbol] != -1) {
         cout << "extend condition 0c" << endl;
         cout << "extend condition 0c, insertion_point: " << insertion_point << endl;
         position = position - store[insertion_point].edge_label_length(); //////////////////////////////////////////////////// bork change removed +1
         position--;
         return extend(store[insertion_point].children[s[symbol_index]],symbol,symbol_index,insertion,position,test,posrem); // bork change was krob above
       } else {
         // there is no child with this symbol, add one!
         SuffixNode newnode(insertion_point,symbol_index-position+1); // -position (make this generalise
         store.push_back(newnode);
         int nidx = store.size()-1;
         store[insertion_point].children[s[symbol_index-position+1]] = nidx;
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
insertion = true;
          if(store[store[insertion_point].parent].children[symbol] != -1) return extend(store[store[insertion_point].parent].children[symbol],symbol,symbol_index,insertion,position,test,posrem);
                                                                     else {
cout << "extend condition 0.1b1 going to parent: " << store[insertion_point].parent << endl;
//return extend(store[insertion_point].parent                        ,symbol,symbol_index,insertion,position,test,posrem);
         SuffixNode newnode(store[insertion_point].parent,(symbol_index-position)); // -position (make this generalise was +1
         store.push_back(newnode);
         int nidx = store.size()-1;
         store[store[insertion_point].parent].children[symbol] = nidx;
  //       posrem = 1;//!??!
         return insertion_point; // BORKENZ nidx;//ok to return this?
}

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
          return extend(store[insertion_point].children[symbol],symbol,symbol_index,insertion,position,test,posrem);
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
      return extend(store[insertion_point].children[symbol],symbol,symbol_index,insertion,position,test,posrem);
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
    if(store[insertion_point].label_start+position <= symbol_index)
    if((store[insertion_point].edge_label_length() > 0) && (s[store[insertion_point].label_start+position] == symbol)) {
      insertion = false;
      cout << "extend condition 4: position: " << position << endl;
      cout << "extend condition 4" << endl;
 ////////////////////////     posrem = position;
      return insertion_point;
    }

    // 5. If the edge label if length >0 and the edge label mismatches at start_position, split the node
    cout << "pre extend condition 5, label length: " << store[insertion_point].edge_label_length() << endl;
    cout << "pre extend condition 5, label start : " << store[insertion_point].label_start << endl;
    cout << "pre extend condition 5, label end   : " << store[insertion_point].label_end << endl;
    if(store[insertion_point].label_start+position <= symbol_index)
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
	n2.suffix_link = 0;//store[insertion_point].suffix_link; BORK0
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

//BORK0	store[n2_idx].suffix_link = n3_idx; // WEIRDNESS TRY

	store[insertion_point].clear_children();
	store[insertion_point].children[mismatch_symbol] = n2_idx;
	store[insertion_point].children[symbol] = n3_idx;
  /////////////////////      return n2_idx;//insertion_point; //return n2_idx;// was n3_idx
 //       return n3_idx;//insertion_point; //return n2_idx;// was n3_idx
 //       return insertion_point; //return n2_idx;// was n3_idx borkb
        if(position == 1) {posrem=1; return n2_idx; } else // BORKCHANGEN!!!!!!!!!!!!
        if(position == 2) {posrem=2; return n2_idx; } else  // GAMMABORK
        // if(position == 2) {posrem=0; return n3_idx; } else  // GAMMABORK
        { posrem=0; return n2_idx;}//probably not 1
        return n2_idx; //return n2_idx;// was n3_idx borkb


        if(position == 1) {posrem=1; return insertion_point; } else // BORKCHANGEN!!!!!!!!!!!!
        if(position == 2) {posrem=2; return insertion_point; } else  // GAMMABORK
        // if(position == 2) {posrem=0; return n3_idx; } else  // GAMMABORK
        { posrem=0; return insertion_point;}//probably not 1
        return insertion_point; //return n2_idx;// was n3_idx borkb
      }
      return insertion_point;
    }

    cout << "************************* VERY BAD GOT HERE BAD" << endl;
    cout << "insertion point: " << insertion_point << endl;
    cout << "symbol_index was: " << symbol_index << endl;
    cout << "position was: " << position << endl;
    exit(0);
    return -1000000;
    */
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
  int posrem;
};

#endif
