#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"

using namespace std;

int main(int argc,char ** argv) {

  string s = argv[1];

  SuffixTree st;

  int v=1;
if(argc > 3) v = atoi(argv[3]);
cout << "v is: " << v << endl;
for(size_t m=0;m<v;m++)
  for(size_t n=0;n<s.size();n++) {
    st.insert(s[n]);
  }

//  st.dump();


  vector<char> t;
  string ss = argv[2];
  cout << "searching for: " << ss << endl;
  for(size_t n=0;n<ss.size();n++) {
    t.push_back(ss[n]);
  }
 // st.dump();
  bool r = st.exists(t);
  if(r == true) cout << "found" << endl;
  if(r == false) cout << "not found" << endl;
 // st.validate_tree();
}
