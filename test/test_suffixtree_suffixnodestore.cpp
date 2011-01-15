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

  for(size_t n=0;n<100;n++) {

    //cout << "LOOP START" << endl;
    SuffixNode sn(0,0,10);
    int count = ((rand()%35)+2);
    for(int n=0;sn.child_count()<count;n++) {
      int symbol = rand()%39;
      int index  = (rand()%10000) + 0x02000000;
      sn.set_child(symbol,index);
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

class IntWrapper {

public:

  IntWrapper() {}

  IntWrapper(int val_in,int  childlist_idx_in) : val(val_in), childlist_idx(childlist_idx_in) {
  }

  bool isvalid() {
    if(val == -1) return false;

    return true;
  }

  int val;
  int childlist_idx;
};

int test_suffixtree_compact_vec(UnitTest &utf) {

  object_store_type<IntWrapper> v;
  map<int32_t,int32_t> m;

  v.push_back(IntWrapper(0 ,6 + 0x20000)); // 0
  v.push_back(IntWrapper(1 ,9 + 0x20000)); // 1
  v.push_back(IntWrapper(2 ,1 + 0x20000)); // 2
  v.push_back(IntWrapper(3 ,1 + 0x20000)); // 3
  v.push_back(IntWrapper(4 ,1 + 0x20000)); // 4
  v.push_back(IntWrapper(-1,1 + 0x20000)); // 5
  v.push_back(IntWrapper(5 ,1 + 0x20000)); // 6
  v.push_back(IntWrapper(-1,1 + 0x20000)); // 7
  v.push_back(IntWrapper(-1,1 + 0x20000)); // 8
  v.push_back(IntWrapper(9 ,1 + 0x20000)); // 9
  v.push_back(IntWrapper(10,1 + 0x20000)); // 10
  v.push_back(IntWrapper(11,1 + 0x20000)); // 11

  compact_vec(v,m,0x020000);
  apply_mapping(v,m);

  utf.test_equality(static_cast<int>(v.size()),static_cast<int>(9));

  utf.test_equality(v.get(5).val,11);
  utf.test_equality(v.get(0).childlist_idx,6 + 0x20000);
  utf.test_equality(v.get(1).childlist_idx,8 + 0x20000);
  utf.test_equality(v.size(),static_cast<size_t>(9));

/*
  for(int j=0;j<100;j++) {
    v.clear();
    m.clear();
    int i=0;
    for(int n=0;n<100;n++) {
      if(rand()%2 == 0) {i++; v.push_back(IntWrapper(-1 ,-1));} // push -1
      v.push_back(IntWrapper(n ,i + 0x02000000)); // push n
      i++;
    }

    compact_vec(v,m,0x02000000);
    apply_mapping(v,m);

  //cout << "postcompact" << endl;
    for(int n=0;n<v.size();n++) {
      utf.test_equality(v.get(n).val,n);
      utf.test_equality(v.get(n).childlist_idx,n + 0x02000000);
  //   cout << "val: " << v.get(n).val << " , " << v.get(n).childlist_idx - 0x02000000 << endl;
    }
    utf.test_equality(static_cast<size_t>(v.get(v.size()-1).val          ),v.size()-1             );
    utf.test_equality(static_cast<size_t>(v.get(v.size()-1).childlist_idx),v.size()-1 + 0x02000000);
  }
*/
}


int test_suffixtree_childliststore(UnitTest &utf) {

  utf.begin_test_set("ChildListStore tests");

  ChildListStore s;

  vector<SymbolPair> p;

  for(int n=0;n<100;n++) {
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
