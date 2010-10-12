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
    int count = ((rand()%35)+2);
    for(int n=0;sn.child_count()<count;n++) {
      int symbol = rand()%39;
      int index  = (rand()%10000) + 0x02000000;
      sn.set_child(symbol,index);
      cout << "adding child: " << symbol << "," << index << endl;
    }

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

int test_suffixtree_childliststore(UnitTest &utf) {

  utf.begin_test_set("ChildListStore tests");

  ChildListStore s;

  vector<SymbolPair> p;

  for(int n=0;n<10000;n++) {
    int c = (rand()%36)+2;

    for(int i=0;p.size()<c;i++) {
      p.push_back(SymbolPair(rand(),rand()));
    }

    int id = s.push_back(p);

    vector<SymbolPair> p1 = s.get_children(id);

    for(int j=0;j<p1.size();j++) {
      utf.test_equality(p[j].index ,p1[j].index);
      utf.test_equality(p[j].symbol,p1[j].symbol);
    }
  }

  utf.end_test_set();
}
