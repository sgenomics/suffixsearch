#include <iostream>
#include <vector>
#include "stringify.h"
#include <string>
#include "ObjectStore.h"
#include "StringStore.h"

using namespace std;

class BinaryNode {

public:

  BinaryNode() : node_0(0), node_1(0) {}

  size_t node_0;
  size_t node_1;
  size_t uri_index;  // URI is only required for leaf nodes, therefore there maybe a more efficient representation...
//  string uri;  // URI is only required for leaf nodes, therefore there maybe a more efficient representation...
  size_t start_index;
  size_t length;

  void dump() {
    cout << "node_0   : " << node_0    << endl;
    cout << "node_1   : " << node_1    << endl;
    cout << "uri_index: " << uri_index << endl;
  }

  void set_uri(string uri) {
    uri_index = s_uri_string_store.add_string(uri);
  }

  string get_uri() {
    return s_uri_string_store.get_string(uri_index);
  }

  static StringStore s_uri_string_store;
};

class URIStore {

public:

  ObjectStore<BinaryNode> dtreestore;

  URIStore() {
    BinaryNode root;
    dtreestore.push_back(root);
  }

  void dump_tree(vector<BinaryNode> &tree) {

    for(size_t n=0;n<tree.size();n++) {
      cout << "node id: " << n << endl;
      tree[n].dump();
    }
  }

  void insert(size_t        value,
              size_t        length,
              const string &uri) {

    size_t current_node = 0;

    for(int n=31;n>=0;n--) {
      int current_bit = value & (1 << n);
      if(current_bit > 0) current_bit = 1;

      size_t o_current_node = current_node;
      if(current_bit) current_node = dtreestore.get(current_node).node_1;
                 else current_node = dtreestore.get(current_node).node_0;


      if(current_node == 0) {
        // need to create a new node and add it here.
        dtreestore.push_back(BinaryNode());

        BinaryNode b_o_current_node = dtreestore.get(o_current_node);

        if(current_bit) b_o_current_node.node_1 = dtreestore.size()-1;
                   else b_o_current_node.node_0 = dtreestore.size()-1;

        dtreestore.set(o_current_node,b_o_current_node);

        current_node = dtreestore.size()-1;
      }
    }

    
    BinaryNode b_current_node = dtreestore.get(current_node);

    b_current_node.set_uri(uri);
    b_current_node.start_index = value;
    b_current_node.length      = length;

    dtreestore.set(current_node,b_current_node);
  }

  // find the nearest prior uri to this index value.
  void find_bounding(size_t index,
                     string &uri,
                     size_t &start_index,
                     size_t &length) {
    size_t current_node = 0;

    size_t last_0_node = 0;

    int n=31;
    for(;n>=0;n--) {
      int current_bit = index & (1 << n);
      if(current_bit > 0) current_bit = 1;

      BinaryNode b_current_node = dtreestore.get(current_node);

      if(b_current_node.node_0 != 0) last_0_node = b_current_node.node_0;
      if(current_bit) current_node = b_current_node.node_1;
                 else current_node = b_current_node.node_0;

      if(current_node == 0) {
        break;
        // no more exact leading bits.
      }
    }

    // move down from last_0_node, always following 1s if possible.
    current_node = last_0_node;
    BinaryNode b_current_node = dtreestore.get(current_node);
    for(;;) {
      b_current_node = dtreestore.get(current_node);

      if((b_current_node.node_1 == 0) && (b_current_node.node_0 == 0)) {
        break;
      }
      
      if(b_current_node.node_1 != 0) {
        current_node = b_current_node.node_1;
      } else {
        current_node = b_current_node.node_0;
      }
    }
    b_current_node = dtreestore.get(current_node);

    uri         = b_current_node.get_uri();
    start_index = b_current_node.start_index;
    length      = b_current_node.length;
  }
};

/*
int main() {

  URIStore myuristore;

  vector<size_t> inserted;

  for(size_t n=0;n<100;n++) {

    size_t r = rand();

    cout << "inserting: " << r << endl;
    inserted.push_back(r);

    myuristore.insert(r,string("http://41j.com/") + stringify(r));
  }

  for(size_t n=0;n<inserted.size();n++) {
    size_t v = inserted[n];
    cout << "searched for: " << stringify(v+100) << endl;
    cout << "        base: " << stringify(v) << endl;
    cout << "       found: " << myuristore.find_bounding(v+100) << endl;
  }
}
*/
