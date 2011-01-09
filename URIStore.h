#include <iostream>
#include <vector>
#include "stringify.h"
#include <string>

using namespace std;

class BinaryNode {

public:

  BinaryNode() : node_0(0), node_1(0) {}

  size_t node_0;
  size_t node_1;
  string uri;  // URI is only required for leaf nodes, therefore there maybe a more efficient representation...
  size_t start_index;
  size_t length;

  void dump() {
    cout << "node_0: " << node_0 << endl;
    cout << "node_1: " << node_1 << endl;
    cout << "   uri: " << uri    << endl;
  } 
};

class URIStore {

public:

  vector<BinaryNode> dtreestore;

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
      if(current_bit) current_node = dtreestore[current_node].node_1;
                 else current_node = dtreestore[current_node].node_0;


      if(current_node == 0) {
        // need to create a new node and add it here.
        dtreestore.push_back(BinaryNode());

        if(current_bit) dtreestore[o_current_node].node_1 = dtreestore.size()-1;
                   else dtreestore[o_current_node].node_0 = dtreestore.size()-1;

        current_node = dtreestore.size()-1;
      }
    }

    dtreestore[current_node].uri = uri;
    dtreestore[current_node].start_index = value;
    dtreestore[current_node].length      = length;
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

      //size_t o_current_node = current_node;
      if(dtreestore[current_node].node_0 != 0) last_0_node = dtreestore[current_node].node_0;
      if(current_bit) current_node = dtreestore[current_node].node_1;
                 else current_node = dtreestore[current_node].node_0;

      if(current_node == 0) {
        break;
        // no more exact leading bits.
      }
    }

    // move down from last_0_node, always following 1s if possible.
    current_node = last_0_node;
    for(;;) {

      if((dtreestore[current_node].node_1 == 0) && (dtreestore[current_node].node_0 == 0)) {
        break;
      }
      
      if(dtreestore[current_node].node_1 != 0) {
        current_node = dtreestore[current_node].node_1;
      } else {
        current_node = dtreestore[current_node].node_0;
      }
    }

    uri         = dtreestore[current_node].uri;
    start_index = dtreestore[current_node].start_index;
    length      = dtreestore[current_node].length;
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
