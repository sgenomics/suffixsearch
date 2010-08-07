#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"

#include "UnitTest.h"

using namespace std;

int test_suffixtree_random(UnitTest &utf) {

  utf.begin_test_set("Random SuffixTree tests");
  SuffixTree s;

  s.insert('b');
  s.insert('a');
  s.insert('n');
  s.insert('a');
  s.insert('n');
  s.insert('a');
  s.finalise();

  // simple existance tests

  vector<char> chkstr;
  chkstr.push_back('a');
  chkstr.push_back('n');
  chkstr.push_back('a');
  bool res;

  res = s.exists(chkstr);
  utf.test_equality(res,true);

  chkstr.clear();
  chkstr.push_back('a');
  chkstr.push_back('a');
  chkstr.push_back('a');
  res = s.exists(chkstr);
  utf.test_equality(res,false);

/*
  s.process_positions();
  s.process_parents();
  cout << "************************************************************** AFTER LEAF PROCESSING" << endl;
  s.dump();

  // test occurs
  vector<char> chkstr2;
  chkstr2.push_back('a');
  chkstr2.push_back('n');
  vector<int> res2 = s.all_occurs(chkstr2);
  cout << "********************************************** occurs count: " << res2.size() << endl;
  if(res2.size() == 1) cout << "test 1B passed" << endl; else cout << "test 1B failed" << endl;
*/
  // randomised tests

  for(int i=0;i<100;i++) {
    vector<char> str2;
    vector<char> chkstr2;

    for(int n=0;n<2000;n++) {
      char c = (rand()%3)+65;
      str2.push_back(c);
    }

    for(int n=0+(rand()%2);n<(8+rand()%1);n++) {
      chkstr2.push_back(str2[n]);
    }
    
    SuffixTree s2;
    for(size_t n=0;n<str2.size();n++) {
      s2.insert(str2[n]);
    }
    s2.finalise();

    bool validation = s2.validate_tree();
    utf.test_equality(validation,true);

    res = s2.exists(chkstr2);
    utf.test_equality(res,true);

    vector<char> chkstr3;
    chkstr3.push_back('z');
    chkstr3.push_back('z');
    chkstr3.push_back('z');
    res = s2.exists(chkstr3);
    utf.test_equality(res,false);

    string s3;
    string s2str;
    for(size_t n=0;n<str2.size();n++) {
      s3 += str2[n];
    }

    vector<char> chkstr4;
    for(int n=0;n<3;n++) {
      char c = (rand()%3)+65;
      s2str += c;
      chkstr4.push_back(c);
    }

    bool findit = false;
    size_t p = s3.find(s2str);
    if(p == string::npos) {
      findit = false;
    } else {
      findit = true;
    }
 
    res = s2.exists(chkstr4);
    utf.test_equality(res,findit);
  }
  utf.end_test_set();

}

