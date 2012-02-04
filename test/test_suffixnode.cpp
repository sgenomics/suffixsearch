#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "UnitTest.h"
#include "SuffixNode.h"
#include "../global_defs.h"

using namespace std;

void test_suffixnode(UnitTest &utf) {

  utf.begin_test_set("SuffixNode tests");

  SuffixNode s;
  s.set_child(1,5);
  s.set_child(2,5);
  s.set_child(1,8);
  s.set_child(7,6);
  s.set_child(3,4);

  utf.test_equality(s.get_child(1),8);
  utf.test_equality(s.get_child(2),5);
  utf.test_equality(s.get_child(3),4);
  utf.test_equality(s.get_child(7),6);
  utf.test_equality(s.get_child(5),-1);

  utf.test_equality(s.child_count(),4);
  utf.test_equality(s.is_child(4324),false);

  int32_t current = s.get_first_child();
  utf.test_equality(current,8);
  current = s.next_child(current);
  utf.test_equality(current,5);
  current = s.next_child(current);
  utf.test_equality(current,6);
  current = s.next_child(current);
  utf.test_equality(current,4);
  utf.test_equality(s.get_last_child(),4);
  

  SuffixNode s1;
  s1.set_child(2,5);
  s1.set_child(1,8);
  s1.set_child(7,5);
  s1.set_child(3,4);

  SuffixNode s2;
  s2.copy_children(s);
  utf.test_equality(s2.get_child(1),8);
  utf.test_equality(s2.get_child(2),5);
  utf.test_equality(s2.get_child(3),4);
  utf.test_equality(s2.get_child(7),6);
  utf.test_equality(s2.get_child(5),-1);

  SuffixNode s3;
  utf.test_equality(s3.is_leaf(),true);
  utf.test_equality(s2.is_leaf(),false);

  s2.clear_children();
  utf.test_equality(s3.is_leaf(),true);
  utf.test_equality(s3.child_count(),0);

  utf.test_equality(s.is_child(5),true);



  SuffixNode s4;
  s.set_child(1,5);
  s.set_child(2,5);
  s.set_child(1,8);
  s.set_child(7,6);
  s.set_child(3,4);
  s.set_child(2,-1);

  utf.test_equality(s.find_child(8),1);
  utf.test_equality(s.find_child(4),3);

  current = s.get_first_child();
  utf.test_equality(current,8);
  current = s.next_child(current);
  utf.test_equality(current,6);
  current = s.next_child(current);
  utf.test_equality(current,4);
  utf.test_equality(s.get_last_child(),4);
  utf.test_equality(s.child_count(),3);

  SuffixNode s5;
  s5.set_child(5,-1);
  utf.test_equality(s5.child_count(),0);


  utf.end_test_set();
}


