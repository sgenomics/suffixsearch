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
#include "ProcessPositions.h"
#include "DiskVector.h"

using namespace std;

int main(int argc,char ** argv) {

  SuffixNodeStoreDisk diskstore(argv[1]);
  SuffixNode::store = &diskstore;

  ProcessPositions<SuffixNodeStoreDisk,SuffixNode> propos(diskstore);
  propos.process_positions();

  DiskVector<positions> dvec(string(argv[1]) + "/process_positions");
  propos.copyto(dvec);
}
