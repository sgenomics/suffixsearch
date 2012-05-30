#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include "./global_defs.h"
#include "./suffixtrans/SearchTrans.h"
#include "WebSrvSearch.h"


using namespace std;

int main(int argc,char ** argv) {

  SearchTrans st;

  WebSrvSearch<SearchTrans> web(st,8080,8081);
  web.document_listener();
  st.finalise();

  cout << "built suffix tree" << endl;

  cout << "processing positions" << endl;
  st.process_positions();
  cout << "processed positions" << endl;
  st.dump_stats();

  web.search_listener();
}
