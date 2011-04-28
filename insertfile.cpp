#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"
#include <stdint.h>

using namespace std;

int main(int argc,char ** argv) {

  ifstream input_file(argv[1]);

  SuffixTree st;
  //st.set_compactmode(false);

  char s;
  for(;!input_file.eof();) {
    s = static_cast<char>(input_file.get());
    st.insert(s);
  }

  st.finalise();
  st.compact();

  vector<char> t;
  string ss = argv[2];
  cout << "searching for: " << ss << endl;
  for(size_t n=0;n<ss.size();n++) {
    t.push_back(ss[n]);
  }

  bool r = st.exists(t);
  if(r == true) cout << "found" << endl;
  if(r == false) cout << "not found" << endl;

  st.process_positions();
  vector<int> poss = st.all_occurs(t);

  st.dump_stats();

  for(int n=0;n<poss.size();n++) cout << "position: " << poss[n] << endl;

}
