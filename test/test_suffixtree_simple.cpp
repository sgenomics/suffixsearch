#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"
#include "UnitTest.h"
#include "global_defs.h"

using namespace std;

int test_suffixtree_simple(UnitTest &utf) {

  utf.begin_test_set("Simple SuffixTree tests");
  int runonly=0;
  bool runall=true;

  if(runonly==-30 || runall) {
    string ms = "AABACBCCCABCAAABAABA";
    SuffixTree s1;

    for(int n=0;n<ms.size();n++) {
      s1.insert(ms[n]);
    }
    s1.finalise();
    bool validation = s1.validate_tree(true);
    utf.test_equality(true,validation);

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('C');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(true,res1);
  }

  if(runonly==-29 || runall) {
    string ms = "BBABCBBAABCBCBCBAABBCCAACCCBBBCAAACABBBBAAACCBCCCACBBCCACCBBAACACCBABCAAAACACCACBAACCAACCBAACABBBAACABCBCACCAABCBCCBACBCBAABACAC";
    SuffixTree s1;

    for(int n=0;n<ms.size();n++) {
      s1.insert(ms[n]);
    }
    s1.finalise();

    bool validation = s1.validate_tree();
    utf.test_equality(true,validation);

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('C');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(true,res1);
  }


  if(runonly==-28 || runall) {
    string ms = "BCCCCCCCCCCCCCC";
    SuffixTree s1;

    for(int n=0;n<ms.size();n++) {
      s1.insert(ms[n]);
    }
    s1.finalise();

    bool validation = s1.validate_tree();
    utf.test_equality(true,validation);

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('C');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(true,res1);
  }

  if(runonly==-27 || runall) {
    string ms = "CACACCACACACACA";
    SuffixTree s1;

    for(int n=0;n<ms.size();n++) {
      s1.insert(ms[n]);
    }
    s1.finalise();

    bool validation = s1.validate_tree();
    utf.test_equality(true,validation);

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('A');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-26 || runall) {
    string ms = "CBACBACBACBCBBA";
    SuffixTree s1;

    for(int n=0;n<ms.size();n++) {
      s1.insert(ms[n]);
    }
    s1.finalise();

    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('A');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-25 || runall) {
    string ms = "ACBBAACACACCACCACCACCCBABCBCABCABACCBCAACAAABAABCBACACACBCBCBAAABACCAAAACBBACCBCABACBBCCAABCBAACAAAB";
    SuffixTree s1;

    for(int n=0;n<ms.size();n++) {
      s1.insert(ms[n]);
    }
    s1.finalise();

    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('A');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-24 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('b');
    s1.insert('a');
    s1.insert('b');
    s1.insert('b');
    s1.insert('a');
    s1.insert('b');
    s1.insert('a');
    s1.insert('b');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('a');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-23 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('a');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-22 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }


  if(runonly==-21 || runall) {
    SuffixTree s1;

    s1.insert('b');
    s1.insert('c');
    s1.insert('c');
    s1.insert('a');
    s1.insert('b');
    s1.insert('c');
    s1.insert('c');
    s1.insert('a');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-20 || runall) {
    SuffixTree s1;

    s1.insert('b');
    s1.insert('c');
    s1.insert('a');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('c');
    s1.insert('a');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-19 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('c');
    s1.insert('c');
    s1.insert('a');
    s1.insert('c');
    s1.insert('c');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-18 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-17 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }


  if(runonly==-16 || runall) {
    SuffixTree s1;

    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-15 || runall) {
    SuffixTree s1;

    s1.insert('c');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('c');
    s1.insert('a');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,false);
  }

  if(runonly==-14 || runall) {
    SuffixTree s1;

    s1.insert('b');
    s1.insert('a');
    s1.insert('b');
    s1.insert('b');
    s1.insert('b');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,false);
  }


  if(runonly==-13 || runall) {
    SuffixTree s1;

    s1.insert('c');
    s1.insert('b');
    s1.insert('a');
    s1.insert('c');
    s1.insert('b');
    s1.insert('c');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,false);
  }

  if(runonly==-12 || runall) {
    SuffixTree s1;

    s1.insert('c');
    s1.insert('a');
    s1.insert('b');
    s1.insert('b');
    s1.insert('c');
    s1.insert('a');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('a');
    chkstr1.push_back('a');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,false);
  }

  if(runonly==-11 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('b');
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('a');
    bool res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-10 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('c');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('a');
    chkstr1.push_back('c');
    bool res1;
    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);

    chkstr1.clear();
    chkstr1.push_back('a');
    chkstr1.push_back('a');
    chkstr1.push_back('a');
    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }


  if(runonly==-9 || runall) {
    SuffixTree s1;

    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('b');
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-8 || runall) {
    SuffixTree s1;

    s1.insert('b');
    s1.insert('b');
    s1.insert('a');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('b');
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }


  if(runonly==-7 || runall) {
    SuffixTree s1;

    s1.insert('b');
    s1.insert('b');
    s1.insert('a');
    s1.insert('a');
    s1.insert('b');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,false);
  }

  if(runonly==-6 || runall) {
    SuffixTree s1;

    s1.insert('b');
    s1.insert('b');
    s1.insert('a');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('b');
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);

    chkstr1.clear();
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);

    chkstr1.clear();
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    chkstr1.push_back('a');
    chkstr1.push_back('b');
    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);

    chkstr1.clear();
    chkstr1.push_back('b');
    chkstr1.push_back('a');
    chkstr1.push_back('b');
    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }


  if(runonly==-5 || runall) {
    SuffixTree s1;

    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.insert('b');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }


  if(runonly==-4 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('b');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);

  }

  if(runonly==-3 || runall) {
    SuffixTree s1;

    s1.insert('b');
    s1.insert('b');
    s1.insert('c');
    s1.insert('c');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,false);

    vector<char> chkstr2;
    chkstr2.push_back('b');
    bool res2;

    res2 = s1.exists(chkstr2);
    utf.test_equality(res2,true);
  }


  if(runonly==-2 || runall) {
    SuffixTree s1;

    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('b');
    s1.insert('c');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('c');
    chkstr1.push_back('c');
    chkstr1.push_back('a');
    chkstr1.push_back('c');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==-1 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.insert('b');
    s1.insert('b');
    s1.insert('a');
    s1.insert('b');
    s1.insert('a');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('a');
    chkstr1.push_back('a');
    chkstr1.push_back('a');
    chkstr1.push_back('c');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }


  if(runonly==0 || runall) {
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('b');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('a');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);
  }

  if(runonly==1 || runall) {
    SuffixTree s1;

    s1.insert('c');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('a');
    s1.finalise();
    bool validation = s1.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    chkstr1.push_back('c');
    bool res1;

    res1 = s1.exists(chkstr1);
    utf.test_equality(res1,true);

  }

  if(runonly==2 || runall) {
    SuffixTree s2;
    s2.insert('b');
    s2.insert('a');
    s2.insert('c');
    s2.insert('a');
    s2.insert('c');
    s2.insert('b');
    s2.insert('a');
    s2.finalise();
    bool validation = s2.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr2;
    chkstr2.push_back('a');
    chkstr2.push_back('c');
    chkstr2.push_back('a');
    chkstr2.push_back('c');
    bool res2;

    res2 = s2.exists(chkstr2);
    utf.test_equality(res2,true);
  }

  if(runonly==3 || runall) {
    SuffixTree s3;
    s3.insert('a');
    s3.insert('b');
    s3.insert('a');
    s3.insert('a');
    s3.finalise();
    bool validation = s3.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr3;
    chkstr3.push_back('a');
    chkstr3.push_back('b');
    chkstr3.push_back('a');
    chkstr3.push_back('a');
    bool res3;

    res3 = s3.exists(chkstr3);
    utf.test_equality(res3,true);
  }

  if(runonly==4 || runall) {
    SuffixTree s4;
    s4.insert('a');
    s4.insert('a');
    s4.finalise();
    bool validation = s4.validate_tree();
    utf.test_equality(validation,true);

    vector<char> chkstr4;
    chkstr4.push_back('a');
    chkstr4.push_back('a');
    bool res4;

    res4 = s4.exists(chkstr4);
    utf.test_equality(res4,true);

    chkstr4.clear();
    chkstr4.push_back('a');
    res4 = s4.exists(chkstr4);
    utf.test_equality(res4,true);

  }

  if(runonly==5 || runall) {
    SuffixTree s5;
    s5.insert('a');
    s5.insert('n');
    s5.insert('a');
    s5.finalise();

    vector<char> chkstr5;
    chkstr5.push_back('n');
    chkstr5.push_back('a');
    bool res5;

    res5 = s5.exists(chkstr5);
    utf.test_equality(res5,true);

    chkstr5.clear();
    chkstr5.push_back('a');
    chkstr5.push_back('n');
    chkstr5.push_back('a');
    res5 = s5.exists(chkstr5);
    utf.test_equality(res5,true);

    chkstr5.clear();
    chkstr5.push_back('a');
    res5 = s5.exists(chkstr5);
    utf.test_equality(res5,true);
  }


  if(runonly==6 || runall) {
    SuffixTree s6;
    s6.insert('a');
    s6.insert('b');
    s6.insert('c');
    s6.insert('a');
    s6.insert('b');
    s6.insert('c');
    s6.insert('a');
    s6.finalise();

    vector<char> chkstr6;
    chkstr6.push_back('a');
    chkstr6.push_back('b');
    chkstr6.push_back('c');
    chkstr6.push_back('a');
    bool res6;

    res6 = s6.exists(chkstr6);
    utf.test_equality(res6,true);

    chkstr6.clear();
    chkstr6.push_back('b');
    chkstr6.push_back('c');
    chkstr6.push_back('a');
    res6 = s6.exists(chkstr6);
    utf.test_equality(res6,true);

    chkstr6.clear();
    chkstr6.push_back('a');
    res6 = s6.exists(chkstr6);
    utf.test_equality(res6,true);

    chkstr6.clear();
    chkstr6.push_back('a');
    chkstr6.push_back('b');
    chkstr6.push_back('c');
    chkstr6.push_back('a');
    res6 = s6.exists(chkstr6);
    utf.test_equality(res6,true);

    chkstr6.clear();
    chkstr6.push_back('a');
    chkstr6.push_back('b');
    chkstr6.push_back('c');
    chkstr6.push_back('a');
    chkstr6.push_back('b');
    chkstr6.push_back('c');
    chkstr6.push_back('a');
    res6 = s6.exists(chkstr6);
    utf.test_equality(res6,true);
  }


  if(runonly==7 || runall) {
    SuffixTree s7;
    s7.insert('a');
    s7.insert('n');
    s7.insert('a');
    s7.insert('n');
    s7.finalise();

    vector<char> chkstr7;
    chkstr7.push_back('a');
    chkstr7.push_back('n');
    chkstr7.push_back('a');
    bool res7;

    res7 = s7.exists(chkstr7);
    utf.test_equality(res7,true);
  }

  if(runonly==8 || runall) {
    SuffixTree s8;
    s8.insert('z');
    s8.insert('a');
    s8.insert('b');
    s8.insert('c');
    s8.insert('a');
    s8.insert('b');
    s8.insert('c');
    s8.insert('a');
    s8.insert('b');
    s8.insert('c');
    s8.finalise();

    vector<char> chkstr8;
    chkstr8.push_back('z');
    chkstr8.push_back('a');
    chkstr8.push_back('b');
    bool res8;

    res8 = s8.exists(chkstr8);
    utf.test_equality(res8,true);

    vector<char> chkstr8a;
    chkstr8a.push_back('a');
    chkstr8a.push_back('b');
    chkstr8a.push_back('c');
    chkstr8a.push_back('a');
    chkstr8a.push_back('b');
    bool res8a;

    res8a = s8.exists(chkstr8a);
    utf.test_equality(res8a,true);
    bool validation = s8.validate_tree();
    utf.test_equality(validation,true);
  }
 
  if(runonly==9 || runall) {
    SuffixTree s9;
    s9.insert('b');
    s9.insert('a');
    s9.insert('n');
    s9.insert('a');
    s9.insert('n');
    s9.insert('a');
    s9.finalise();

    vector<char> chkstr9;
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    bool res;

    res = s9.exists(chkstr9);
    utf.test_equality(res,true);

    chkstr9.clear();
    chkstr9.push_back('b');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    utf.test_equality(res,true);

    chkstr9.clear();
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    utf.test_equality(res,true);

    chkstr9.clear();
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    utf.test_equality(res,true);

    chkstr9.clear();
    chkstr9.push_back('a');
    chkstr9.push_back('a');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    utf.test_equality(res,false);
    bool validation = s9.validate_tree();
    utf.test_equality(validation,true);
  }

  if(runall) {
    SuffixTree s9;
    s9.insert(200);
    s9.insert(100);
    s9.insert('n');
    s9.insert(50);
    s9.insert(1);
    s9.insert(160);
    s9.finalise();

    vector<char> chkstr9;
    chkstr9.push_back(200);
    chkstr9.push_back(100);
    chkstr9.push_back('n');
    bool res;

    res = s9.exists(chkstr9);
    utf.test_equality(res,true);
  }

  utf.end_test_set();

}
