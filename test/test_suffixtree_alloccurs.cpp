#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"
#include "UnitTest.h"

using namespace std;

int test_suffixtree_alloccurs(UnitTest &utf) {

  utf.begin_test_set("All Occurs SuffixTree tests");

  string ms = "banana$";
  SuffixTree s1;

  for(int n=0;n<ms.size();n++) {
    s1.insert(ms[n]);
  }

  bool validation = s1.validate_tree();
  utf.test_equality(true,validation);

  s1.process_positions();
  validation = s1.validate_positions();
  utf.test_equality(true,validation);

  vector<char> ss;
  ss.push_back('n');
  ss.push_back('a');
  vector<int> p = s1.all_occurs(ss);

  utf.test_equality(static_cast<size_t>(2),p.size());

  utf.end_test_set();
}
