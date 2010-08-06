#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"
#include "WebSrvSearch.h"
#include <fstream>

using namespace std;

int main(int argc,char ** argv) {

  string s = argv[1];

  ifstream infile(argv[1]);


  SuffixTree st;

  for(;!infile.eof();) {
    char c;
    infile.get(c);
    st.insert(c);
  }


  st.process_positions();
  st.dump_stats();
  WebSrvSearch web(st,1100);

  web.start();
}
