#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include "./global_defs.h"
#include "../suffixtrans/SearchTrans.h"
#include "../WebSrvSearch.h"

using namespace std;

int main(int argc,char ** argv) {

  if(argc < 2) {
    cerr << "web_disk <index folder>" << endl;
    return -1;
  }

  SearchTrans st;

  // Populate members, set disk based storage.

  WebSrvSearch<SearchTrans> web(st,8080,8081);
  web.search_listener();
}
