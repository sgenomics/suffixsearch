#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"
#include "UnitTest.h"

using namespace std;

int test_suffixtree_processpositions(UnitTest &utf) {

  utf.begin_test_set("Process Positions SuffixTree tests");

  string ms = "banana$";
  SuffixTree s1;

  for(int n=0;n<ms.size();n++) {
    s1.insert(ms[n]);
  }

  bool validation = s1.validate_tree();
  utf.test_equality(true,validation);

  s1.process_positions();
  //s1.dump();
  validation = s1.validate_positions();
  utf.test_equality(true,validation);

  utf.end_test_set();
}
