#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"

using namespace std;

int main(int argc,char ** argv) {

  string s = argv[1];

  SuffixTree st;

  for(size_t n=0;n<s.size();n++) {
    st.insert(s[n]);
  }

  st.dump();


  vector<char> t;
  string ss = argv[2];
  cout << "searching for: " << ss << endl;
  for(size_t n=0;n<ss.size();n++) {
    t.push_back(ss[n]);
  }
  st.dump();
  st.exists(t);
  st.validate_tree();
}
