#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"
#include "UnitTest.h"
#include <algorithm>

using namespace std;

int test_suffixtree_alloccurs(UnitTest &utf) {

  utf.begin_test_set("All Occurs SuffixTree tests");

  string ms = "banana";
  SuffixTree s1;

  for(int n=0;n<ms.size();n++) {
    s1.insert(ms[n]);
  }
  s1.finalise();

  bool validation = s1.validate_tree();
  utf.test_equality(true,validation);

  s1.process_positions();
  s1.dump();
  validation = s1.validate_positions();
 // utf.test_equality(true,validation);

  vector<char> ss;
  ss.push_back('n');
  ss.push_back('a');
  vector<int> p = s1.all_occurs(ss);

  utf.test_equality(static_cast<size_t>(2),p.size());

  string m2 = "zzzcoopcoopcoop";
  SuffixTree s2;
  for(int n=0;n<m2.size();n++) {
    s2.insert(m2[n]);
  }
  s2.finalise();
  s2.process_positions();

  ss.clear();
  ss.push_back('c');
  ss.push_back('o');
  ss.push_back('o');
  ss.push_back('p');
  p = s2.all_occurs(ss);


  utf.test_equality(p.size(),static_cast<size_t>(3));
  // should find: 3,7,11
  vector<int>::iterator i = std::find(p.begin(), p.end(), 3);
  utf.test_truth(i != p.end());
  i = std::find(p.begin(), p.end(), 7);
  utf.test_truth(i != p.end());
  i = std::find(p.begin(), p.end(), 11);
  utf.test_truth(i != p.end());



  utf.end_test_set();
}
