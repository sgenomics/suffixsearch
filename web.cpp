#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"
#include "WebSrvSearch.h"
#include <fstream>

using namespace std;

int main(int argc,char ** argv) {

  SuffixTree st;

  WebSrvSearch web(st,8080,8081);
  web.document_listener();

  st.finalise();
  cout << "built suffix tree" << endl;

  cout << "processing positions" << endl;
  st.process_positions();
  cout << "processed positions" << endl;
  st.dump_stats();

  web.search_listener();
}
