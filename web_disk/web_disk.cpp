#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include "./global_defs.h"
#include "../suffixtrans/SearchTrans.h"
#include "../WebSrvSearch.h"
#include "SuffixTree.h"
#include "DiskVector.h"
#include "TranscodingDiskVector.h"

using namespace std;

int main(int argc,char ** argv) {

  if(argc < 2) {
    cerr << "web_disk <index folder>" << endl;
    return -1;
  }

  SearchTrans st(true);

  // Populate members, set disk based storage.
  st.load_members(string(argv[1]) + "/object_members");
  st.st.store = SuffixNodeStoreDisk(string(argv[1]));
  SuffixNode::store = &(st.st.store);

  st.st.s = TranscodingDiskVector(string(argv[1]) + "/original_text");
  st.original_text = DiskVector<uint8_t>(string(argv[1]) + "/original_text");

  DiskVector<positions> pos_store(string(argv[1]) + "/process_positions");
  ProcessPositions<SuffixNodeStoreDisk,SuffixNode> pp(st.st.store,pos_store);

  st.dump_stats();

  WebSrvSearch<SearchTrans,ProcessPositions<SuffixNodeStoreDisk,SuffixNode> > web(st,pp,8080,8081);
  web.search_listener();
}
