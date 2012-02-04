#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "UnitTest.h"
#include "SuffixNode.h"

using namespace std;

void test_suffixnode(UnitTest &utf) {

  utf.begin_test_set("SuffixNode tests");

  SuffixNode s;

  s.set_child(1,5);
  s.set_child(2,5);
  s.set_child(1,8);
  s.set_child(7,5);
  s.set_child(3,4);

  utf.test_equality(s.get_child(1),8);
  utf.test_equality(s.get_child(2),5);
  utf.test_equality(s.get_child(3),4);
  utf.test_equality(s.get_child(7),5);
  utf.test_equality(s.get_child(5),-1);

  utf.test_equality(s.first_child(),1);

  SuffixNode s1;
  s1.set_child(2,5);
  s1.set_child(1,8);
  s1.set_child(7,5);
  s1.set_child(3,4);
  utf.test_equality(s1.first_child(),1);

  SuffixNode s2;
  s2.copy_children(s);
  utf.test_equality(s2.get_child(1),8);
  utf.test_equality(s2.get_child(2),5);
  utf.test_equality(s2.get_child(3),4);
  utf.test_equality(s2.get_child(7),5);
  utf.test_equality(s2.get_child(5),-1);

  s2.replace_children(4,100);
  utf.test_equality(s2.get_child(3),100);

  utf.test_equality(s2.find_child(100),3);

  utf.end_test_set();
}
