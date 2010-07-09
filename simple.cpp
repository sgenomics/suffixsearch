#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"

using namespace std;

int main(int argc,char ** argv) {

  int runonly=-11;
  bool runall=false;

  if(argc > 1) {
    if(argv[1][0] == 'A') {runall=true; cout << "runall" << endl; }
    else { runonly = atoi(argv[1]); cout << "runonly " << runonly << endl; }
  }


  if(runonly==-25 || runall) {
    cout << "RUN -25" << endl;
    string ms = "ACBBAACACACCACCACCACCCBABCBCABCABACCBCAACAAABAABCBACACACBCBCBAAABACCAAAACBBACCBCABACBBCCAABCBAACAAAB$";
    SuffixTree s1;

    for(int n=0;n<ms.size();n++) {
      s1.insert(ms[n]);
    }

    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('a');
    bool res1 = s1.exists(chkstr1);
    if(res1 != true) cout << "******************************* TEST -25 FAILURE" << endl;
  }

  if(runonly==-24 || runall) {
    cout << "RUN -24" << endl;
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
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('a');
    bool res1 = s1.exists(chkstr1);
    if(res1 != true) cout << "******************************* TEST -24 FAILURE" << endl;
  }

  if(runonly==-23 || runall) {
    cout << "RUN -23" << endl;
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('a');
    bool res1 = s1.exists(chkstr1);
    if(res1 != true) cout << "******************************* TEST -23 FAILURE" << endl;
  }

  if(runonly==-22 || runall) {
    cout << "RUN -22" << endl;
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    if(res1 != true) cout << "******************************* TEST -22 FAILURE" << endl;
  }


  if(runonly==-21 || runall) {
    cout << "RUN -21" << endl;
    SuffixTree s1;

    s1.insert('b');
    s1.insert('c');
    s1.insert('c');
    s1.insert('a');
    s1.insert('b');
    s1.insert('c');
    s1.insert('c');
    s1.insert('a');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    if(res1 != true) cout << "******************************* TEST -21 FAILURE" << endl;
  }

  if(runonly==-20 || runall) {
    cout << "RUN -20" << endl;
    SuffixTree s1;

    s1.insert('b');
    s1.insert('c');
    s1.insert('a');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('c');
    s1.insert('a');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    if(res1 != true) cout << "******************************* TEST -20 FAILURE" << endl;
  }

  if(runonly==-19 || runall) {
    cout << "RUN -19" << endl;
    SuffixTree s1;

    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('c');
    s1.insert('c');
    s1.insert('a');
    s1.insert('c');
    s1.insert('c');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    if(res1 != true) cout << "******************************* TEST -19 FAILURE" << endl;
  }

  if(runonly==-18 || runall) {
    cout << "RUN -18" << endl;
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    if(res1 != true) cout << "******************************* TEST -18 FAILURE" << endl;
  }

  if(runonly==-17 || runall) {
    cout << "RUN -17" << endl;
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    if(res1 != true) cout << "******************************* TEST -17 FAILURE" << endl;
  }


  if(runonly==-16 || runall) {
    cout << "RUN -16" << endl;
    SuffixTree s1;

    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    bool res1 = s1.exists(chkstr1);
    if(res1 != true) cout << "******************************* TEST -16 FAILURE" << endl;
  }

  if(runonly==-15 || runall) {
    cout << "RUN -15" << endl;
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
    s1.insert('$');
    s1.dump();
    cout << "final insert $ VAL" << endl;
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1 = s1.exists(chkstr1);
    if(res1 == true) cout << "******************************* TEST -15 FAILURE" << endl;
  }

  if(runonly==-14 || runall) {
    cout << "RUN -14" << endl;
    SuffixTree s1;

    s1.insert('b');
    cout << "insert b VAL" << endl;
    s1.validate_tree();
    s1.insert('a');
    cout << "insert a VAL" << endl;
    s1.validate_tree();
    s1.insert('b');
    cout << "insert b VAL" << endl;
    s1.validate_tree();
    s1.insert('b');
    cout << "insert b VAL" << endl;
    s1.validate_tree();
    s1.insert('b');
    cout << "insert b VAL" << endl;
    s1.validate_tree();
    s1.insert('b');
    cout << "insert b VAL" << endl;
    s1.validate_tree();
    s1.insert('$');
    s1.dump();
    cout << "final insert $ VAL" << endl;
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1 = s1.exists(chkstr1);
    if(res1 == true) cout << "******************************* TEST -14 FAILURE" << endl;
  }


  if(runonly==-13 || runall) {
    cout << "RUN -13" << endl;
    SuffixTree s1;

    s1.insert('c');
    s1.insert('b');
    s1.insert('a');
    s1.insert('c');
    s1.insert('b');
    s1.insert('c');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1 = s1.exists(chkstr1);
    if(res1 == true) cout << "******************************* TEST -13 FAILURE" << endl;
  }

  if(runonly==-12 || runall) {
    cout << "RUN -12" << endl;
    SuffixTree s1;

    s1.insert('c');
    s1.insert('a');
    s1.insert('b');
    s1.insert('b');
    s1.insert('c');
    s1.insert('a');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('c');
    chkstr1.push_back('a');
    chkstr1.push_back('a');
    bool res1 = s1.exists(chkstr1);
    if(res1 == true) cout << "******************************* TEST -12 FAILURE" << endl;
  }

  if(runonly==-11 || runall) {
    cout << "RUN -11" << endl;
    SuffixTree s1;

    s1.insert('a');
    s1.insert('b');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.clear();
    chkstr1.push_back('a');
    bool res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -11 FAILURE" << endl;
  }

  if(runonly==-10 || runall) {
//rand str: AAACAC$
//s2str: *CAC*
    cout << "RUN -10" << endl;
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('a');
    s1.insert('c');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('a');
    chkstr1.push_back('c');
    chkstr1.push_back('$');
    bool res1;
    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -10a FAILURE" << endl;

    chkstr1.clear();
    chkstr1.push_back('a');
    chkstr1.push_back('a');
    chkstr1.push_back('a');
    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -10b FAILURE" << endl;
  }


  if(runonly==-9 || runall) {
//rand str: BCBBCB$
//s2str: *BCB*
    cout << "RUN -9" << endl;
    SuffixTree s1;

    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('b');
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('$');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -9 FAILURE" << endl;
  }

  if(runonly==-8 || runall) {
//rand str: BBABCB$
//s2str: *BCB*
    cout << "RUN -8" << endl;
    SuffixTree s1;

    s1.insert('b');
    s1.insert('b');
    s1.insert('a');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('b');
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('$');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -8 FAILURE" << endl;
  }


  if(runonly==-7 || runall) {
//rand str: BBAABB$
//s2str: *BBB*
    cout << "RUN -7" << endl;
    SuffixTree s1;

    s1.insert('b');
    s1.insert('b');
    s1.insert('a');
    s1.insert('a');
    s1.insert('b');
    s1.insert('b');
    s1.insert('$');
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == true) cout << "******************************* TEST -7 FAILURE" << endl;
  }

  if(runonly==-6 || runall) {
//rand str: BBABCB$
//s2str: *BCB*
    cout << "RUN -6" << endl;
    SuffixTree s1;

    s1.insert('b');
    s1.insert('b');
    s1.insert('a');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('b');
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('$');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -6a FAILURE" << endl;

    chkstr1.clear();
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('$');
    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -6b FAILURE" << endl;

    chkstr1.clear();
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    chkstr1.push_back('a');
    chkstr1.push_back('b');
    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -6b FAILURE" << endl;

    chkstr1.clear();
    chkstr1.push_back('b');
    chkstr1.push_back('a');
    chkstr1.push_back('b');
    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -6b FAILURE" << endl;
  }


  if(runonly==-5 || runall) {
//rand str: CCCCBB$
//s2str: *CBB*
    cout << "RUN -5" << endl;
    SuffixTree s1;

    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.insert('c');
    s1.insert('b');
    s1.insert('b');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -5a FAILURE" << endl;
  }


  if(runonly==-4 || runall) {
//rand str: AAACBB$
//s2str: *CBB*
    cout << "RUN -4" << endl;
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('c');
    s1.insert('b');
    s1.insert('b');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -4a FAILURE" << endl;

  }

//rand str: BBCCB$
//s2str: *BBB*
  if(runonly==-3 || runall) {
    cout << "RUN -3" << endl;
    SuffixTree s1;

    s1.insert('b');
    s1.insert('b');
    s1.insert('c');
    s1.insert('c');
    s1.insert('b');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == true) cout << "******************************* TEST -3a FAILURE" << endl;

    vector<char> chkstr2;
    chkstr2.push_back('b');
    chkstr2.push_back('$');
    bool res2;

    res2 = s1.exists(chkstr2);
    if(res2 == false) cout << "******************************* TEST -3b FAILURE" << endl;
  }


//tr: CCCACACABC$
//chk  str: CCCAC
  if(runonly==-2 || runall) {
    cout << "RUN -2" << endl;
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
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('c');
    chkstr1.push_back('c');
    chkstr1.push_back('a');
    chkstr1.push_back('c');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -2 FAILURE" << endl;
  }

  if(runonly==-1 || runall) {
    cout << "RUN -1" << endl;
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
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('a');
    chkstr1.push_back('a');
    chkstr1.push_back('a');
    chkstr1.push_back('c');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST -1 FAILURE" << endl;
  }


  if(runonly==0 || runall) {
    cout << "RUN 0" << endl;
    SuffixTree s1;

    s1.insert('a');
    s1.insert('a');
    s1.insert('a');
    s1.insert('b');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('a');
    chkstr1.push_back('b');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST 0 FAILURE" << endl;
  }

  if(runonly==1 || runall) {
    cout << "RUN 1" << endl;
    SuffixTree s1;

    s1.insert('c');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('b');
    s1.insert('c');
    s1.insert('b');
    s1.insert('a');
    s1.insert('$');
    s1.dump();
    s1.validate_tree();

    vector<char> chkstr1;
    chkstr1.push_back('c');
    chkstr1.push_back('b');
    chkstr1.push_back('b');
    chkstr1.push_back('c');
    bool res1;

    res1 = s1.exists(chkstr1);
    if(res1 == false) cout << "******************************* TEST 1 FAILURE" << endl;

  }
//rand str: CBCBBCBA$
//chk  str: CBBC

  if(runonly==2 || runall) {
    cout << "RUN 2" << endl;
    SuffixTree s2;
    s2.insert('b');
    s2.insert('a');
    s2.insert('c');
    s2.insert('a');
    s2.insert('c');
    s2.insert('b');
    s2.insert('a');
    s2.insert('$');
    s2.dump();
    s2.validate_tree();

    vector<char> chkstr2;
    chkstr2.push_back('a');
    chkstr2.push_back('c');
    chkstr2.push_back('a');
    chkstr2.push_back('c');
    bool res2;

    res2 = s2.exists(chkstr2);
    if(res2 == false) cout << "************************** TEST 2 FAILURE" << endl;
  }

  if(runonly==3 || runall) {
    cout << "RUN 3" << endl;
    SuffixTree s3;
    s3.insert('a');
    s3.insert('b');
    s3.insert('a');
    s3.insert('a');
    s3.insert('$');
    s3.dump();
    s3.validate_tree();
    vector<char> chkstr3;
    chkstr3.push_back('a');
    chkstr3.push_back('b');
    chkstr3.push_back('a');
    chkstr3.push_back('a');
    bool res3;

    res3 = s3.exists(chkstr3);
    if(res3 == false) cout << "************************** TEST 3 FAILURE" << endl;
  }

/*
  s.insert('a');
  s.dump();
  s.insert('b');
  s.dump();
  s.insert('$');
  s.dump();
*/

  if(runonly==4 || runall) {
    cout << "RUN 4" << endl;
    SuffixTree s4;
    s4.insert('a');
    s4.insert('a');
    s4.insert('$');
    s4.dump();
    s4.validate_tree();
    vector<char> chkstr4;
    chkstr4.push_back('a');
    chkstr4.push_back('a');
    bool res4;

    res4 = s4.exists(chkstr4);
    if(res4 == false) cout << "************************** TEST 4a FAILURE" << endl;

    chkstr4.clear();
    chkstr4.push_back('a');
    chkstr4.push_back('$');
    res4 = s4.exists(chkstr4);
    if(res4 == false) cout << "************************** TEST 4b FAILURE" << endl;

    chkstr4.clear();
    chkstr4.push_back('$');
    res4 = s4.exists(chkstr4);
    if(res4 == false) cout << "************************** TEST 4c FAILURE" << endl;
  }

  if(runonly==5 || runall) {
    cout << "RUN 5" << endl;
    SuffixTree s5;
    s5.insert('a');
    s5.dump();
    s5.insert('n');
    s5.dump();
    s5.insert('a');
    s5.dump();
    s5.insert('$');
    s5.dump();

    vector<char> chkstr5;
    chkstr5.push_back('n');
    chkstr5.push_back('a');
    bool res5;

    res5 = s5.exists(chkstr5);
    if(res5 == false) cout << "************************** TEST 5a FAILURE" << endl;

    chkstr5.clear();
    chkstr5.push_back('a');
    chkstr5.push_back('n');
    chkstr5.push_back('a');
    res5 = s5.exists(chkstr5);
    if(res5 == false) cout << "************************** TEST 5b FAILURE" << endl;

    chkstr5.clear();
    chkstr5.push_back('a');
    res5 = s5.exists(chkstr5);
    if(res5 == false) cout << "************************** TEST 5c FAILURE" << endl;
  }


  if(runonly==6 || runall) {
    cout << "RUN 6" << endl;
    SuffixTree s6;
    s6.insert('a');
    s6.dump();
    s6.insert('b');
    s6.dump();
    s6.insert('c');
    s6.dump();
    s6.insert('a');
    s6.dump();
    s6.insert('b');
    s6.dump();
    s6.insert('c');
    s6.dump();
    s6.insert('a');
    s6.dump();
    s6.insert('$');
    s6.dump();

    vector<char> chkstr6;
    chkstr6.push_back('a');
    chkstr6.push_back('b');
    chkstr6.push_back('c');
    chkstr6.push_back('a');
    bool res6;

    res6 = s6.exists(chkstr6);
    if(res6 == false) cout << "************************** TEST 6a FAILURE" << endl;

    chkstr6.clear();
    chkstr6.push_back('b');
    chkstr6.push_back('c');
    chkstr6.push_back('a');
    res6 = s6.exists(chkstr6);
    if(res6 == false) cout << "************************** TEST 6b FAILURE" << endl;

    chkstr6.clear();
    chkstr6.push_back('a');
    chkstr6.push_back('$');
    res6 = s6.exists(chkstr6);
    if(res6 == false) cout << "************************** TEST 6c FAILURE" << endl;

    chkstr6.clear();
    chkstr6.push_back('a');
    chkstr6.push_back('b');
    chkstr6.push_back('c');
    chkstr6.push_back('a');
    chkstr6.push_back('$');
    res6 = s6.exists(chkstr6);
    if(res6 == false) cout << "************************** TEST 6d FAILURE" << endl;

    chkstr6.clear();
    chkstr6.push_back('a');
    chkstr6.push_back('b');
    chkstr6.push_back('c');
    chkstr6.push_back('a');
    chkstr6.push_back('b');
    chkstr6.push_back('c');
    chkstr6.push_back('a');
    chkstr6.push_back('$');
    res6 = s6.exists(chkstr6);
    if(res6 == false) cout << "************************** TEST 6e FAILURE" << endl;
  }


  if(runonly==7 || runall) {
    cout << "RUN 7" << endl;
    SuffixTree s7;
    s7.insert('a');
    s7.dump();
    s7.insert('n');
    s7.dump();
    s7.insert('a');
    s7.dump();
    s7.insert('n');
    s7.dump();
    s7.insert('$');
    s7.dump();

    vector<char> chkstr7;
    chkstr7.push_back('a');
    chkstr7.push_back('n');
    chkstr7.push_back('a');
    bool res7;

    res7 = s7.exists(chkstr7);
    if(res7 == false) cout << "************************** TEST 7 FAILURE" << endl;
  }

  if(runonly==8 || runall) {
    cout << "RUN 8" << endl;
    SuffixTree s8;
    s8.insert('z');
    s8.dump();
    s8.insert('a');
    s8.dump();
    s8.insert('b');
    s8.dump();
    s8.insert('c');
    s8.dump();
    s8.insert('a');
    s8.dump();
    s8.insert('b');
    s8.dump();
    s8.insert('c');
    s8.dump();
    s8.insert('a');
    s8.dump();
    s8.insert('b');
    s8.dump();
    s8.insert('c');
    s8.dump();
    s8.insert('$');
    s8.dump();

    vector<char> chkstr8;
    chkstr8.push_back('z');
    chkstr8.push_back('a');
    chkstr8.push_back('b');
    bool res8;

    res8 = s8.exists(chkstr8);
    if(res8 == false) cout << "************************** TEST 8 FAILURE" << endl;

    vector<char> chkstr8a;
    chkstr8a.push_back('a');
    chkstr8a.push_back('b');
    chkstr8a.push_back('c');
    chkstr8a.push_back('a');
    chkstr8a.push_back('b');
    bool res8a;

    res8a = s8.exists(chkstr8a);
    if(res8a == false) cout << "************************** TEST 8 FAILURE" << endl;
    s8.validate_tree();
  }

 
  if(runonly==9 || runall) {
    cout << "RUN 9" << endl;
    SuffixTree s9;
    s9.insert('b');
    s9.dump();
    s9.insert('a');
    s9.dump();
    s9.insert('n');
    s9.dump();
    s9.insert('a');
    s9.dump();
    s9.insert('n');
    s9.dump();
    s9.insert('a');
    s9.dump();
    s9.insert('$');
    s9.dump();

    vector<char> chkstr9;
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    bool res;

    res = s9.exists(chkstr9);
    if(res == true) cout << "test 9 passed" << endl; else cout << "test 9A FAILURE" << endl;

    chkstr9.clear();
    chkstr9.push_back('b');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    if(res == true) cout << "test 9 passed" << endl; else cout << "test 9B FAILURE" << endl;

    chkstr9.clear();
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    if(res == true) cout << "test 9 passed" << endl; else cout << "test 9C FAILURE" << endl;

    chkstr9.clear();
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    if(res == true) cout << "test 9 passed" << endl; else cout << "test 9D FAILURE" << endl;

    chkstr9.clear();
    chkstr9.push_back('a');
    chkstr9.push_back('a');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    if(res == false) cout << "test 9 passed" << endl; else cout << "test 9E FAILURE" << endl;
  }


}
