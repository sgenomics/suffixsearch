/* -
 * Copyright (c) 2012 Nava Whiteford <new@sgenomics.org>
 * suffixsearch - search suffixtree algorithms
 *
 * A license to use this software is granted to users given access to the git repository at: https://github.com/sgenomics/suffixsearch
 * A complete copy of this license is located in the license.txt file of this package.
 *
 * In Summary this software:
 *
 * Can be used for creating unlimited applications.
 * Can be distributed in binary or object form only.
 * Commercial use is allowed.
 * Can modify source-code but cannot distribute modifications (derivative works).
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "global_defs.h"
#include <stdint.h>
#include <fstream>
#include "SearchTrans.h"
#include "SuffixNodeStoreDisk.h"
#include "DiskVector.h" 
#include <time.h>


using namespace std;

int main(int argc,char ** argv) {

  ifstream input_filelist(argv[1]);

  SearchTrans *stp = new SearchTrans();
  SearchTrans &st = *stp;
  //SuffixTree st;

  ofstream indexfile((string(argv[2]) + "/document_index").c_str());

  for(;!input_filelist.eof();) {
    string filename;
    input_filelist >> filename;

    if(input_filelist.eof()) break;
    indexfile << filename << " " << st.size();


    ifstream input_file(filename.c_str());
    unsigned char s;
    for(;!input_file.eof();) {
      s = static_cast<char>(input_file.get());
      if(!input_file.eof()) st.insert(s);
    }
    indexfile << " " << st.size() << endl;
  }


  st.finalise();
  st.compact();
  st.dump_stats();

//  tialloc::instance()->dump_stats();

  cout << "store writing " << time(NULL) << endl;
  SuffixNodeStoreDisk diskstore(argv[2],true);
  diskstore.copy(st.get_store());
  cout << "store write complete " << time(NULL) << endl;
  st.save_members(string(argv[2]) + "/object_members");
  cout << "member write complete " << time(NULL) << endl;
//  st.save_original_text(string(argv[2]) + "/original_text");
//  cout << "original text write complete" << endl;

  cout << "processing positions " << time(NULL) << endl;
  ProcessPositions<SuffixNodeStoreMemVec,SuffixNode &> propos(st.st.store);
  propos.process_positions();

  cout << "writing processed positions " << time(NULL) << endl;
  DiskVector<positions> dvec(string(argv[2]) + "/process_positions",true);
  propos.copyto(dvec);
  cout << "writen processed positions " << time(NULL) << endl;

  delete stp;
}
