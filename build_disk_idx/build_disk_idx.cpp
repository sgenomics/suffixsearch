/* -
 * Copyright (c) 2012 Nava Whiteford <new@sgenomics.org>
 * suffixcore - core suffixtree algorithms
 *
 * A license to use this software is granted to users given access to the git repository at: https://github.com/sgenomics/suffixcore
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

using namespace std;

int main(int argc,char ** argv) {

  ifstream input_file(argv[1]);

  SearchTrans st;

  unsigned char s;
  for(;!input_file.eof();) {
    s = static_cast<char>(input_file.get());
    if(!input_file.eof()) st.insert(s);
  }

  st.finalise();
  st.compact();
  st.process_positions();

  SuffixNodeStoreDisk diskstore(argv[2]);
  diskstore.copy(st.get_store());
  st.save_members(string(argv[2]) + "/object_members");
  st.save_original_text(string(argv[2]) + "/original_text");
}
