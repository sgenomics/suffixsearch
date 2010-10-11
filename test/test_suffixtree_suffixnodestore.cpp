#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixNodeStore.h"
#include "UnitTest.h"

using namespace std;
int test_suffixtree_suffixnodestore(UnitTest &utf) {

  utf.begin_test_set("SuffixNodeStore tests");

  SuffixNodeStore s;

  SuffixNode psn(0,0,10);
  psn.parent      = 0;
  psn.label_start = 0;
  psn.label_end   = 0;
  psn.depth       = -1;

  int pid = s.push_back(psn);

  for(size_t n=0;n<10000;n++) {

    SuffixNode sn(0,0,10);
    sn.set_child(rand()%36,(rand()%10000) + 0x02000000);
    sn.set_child(rand()%36,(rand()%10000) + 0x02000000);
    sn.set_child(rand()%36,(rand()%10000) + 0x02000000);
    sn.set_child(rand()%36,(rand()%10000) + 0x02000000);


    sn.parent      = pid;
    sn.label_start = 0;
    sn.label_end   = 0;
    sn.depth       = 0;

    int id = s.push_back(sn);

    s.set(id,sn);

    SuffixNode sn2 = s.get(id);

    utf.test_truth(sn.equal(sn2,true));
  }

  utf.end_test_set();
}