#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"

using namespace std;

int main() {

  int runonly=1;
  bool runall=false;

  if(runonly==1 || runall) {
    SuffixTree s1;

    s1.insert('c');
    s1.dump();
    s1.insert('b');
    s1.dump();
    s1.insert('c');
    s1.dump();
    s1.insert('b');
    s1.dump();
    s1.insert('b');
    s1.dump();
    s1.insert('c');
    s1.dump();
    s1.insert('b');
    s1.dump();
    s1.insert('a');
    s1.dump();
    s1.insert('$');
    s1.dump();

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
    SuffixTree s2;
    s2.insert('b');
    s2.dump();
    s2.insert('a');
    s2.dump();
    s2.insert('c');
    s2.dump();
    s2.insert('a');
    s2.dump();
    s2.insert('c');
    s2.dump();
    s2.insert('b');
    s2.dump();
    s2.insert('a');
    s2.dump();
    s2.insert('$');
    s2.dump();

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
    SuffixTree s3;
    s3.insert('a');
    s3.dump();
    s3.insert('b');
    s3.dump();
    s3.insert('a');
    s3.dump();
    s3.insert('a');
    s3.dump();
    s3.insert('$');
    s3.dump();
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
    SuffixTree s4;
    s4.insert('a');
    s4.dump();
    s4.insert('a');
    s4.dump();
    s4.insert('$');
    s4.dump();
    vector<char> chkstr4;
    chkstr4.push_back('a');
    chkstr4.push_back('a');
    bool res4;

    res4 = s4.exists(chkstr4);
    if(res4 == false) cout << "************************** TEST 4 FAILURE" << endl;
  }


  if(runonly==5 || runall) {
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
    if(res5 == false) cout << "************************** TEST 5 FAILURE" << endl;
  }


  if(runonly==6 || runall) {
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
    if(res6 == false) cout << "************************** TEST 6 FAILURE" << endl;

    vector<char> chkstr6a;
    chkstr6a.push_back('b');
    chkstr6a.push_back('c');
    chkstr6a.push_back('a');
    bool res6a;

    res6a = s6.exists(chkstr6a);
    if(res6a == false) cout << "************************** TEST 6a FAILURE" << endl;
  }


  if(runonly==7 || runall) {
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
  }

 
  if(runonly==9 || runall) {
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
    if(res == true) cout << "test 9 passed" << endl; else cout << "test 9 FAILURE" << endl;

    chkstr9.clear();
    chkstr9.push_back('b');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    if(res == true) cout << "test 9 passed" << endl; else cout << "test 9 FAILURE" << endl;

    chkstr9.clear();
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    if(res == true) cout << "test 9 passed" << endl; else cout << "test 9 FAILURE" << endl;

    chkstr9.clear();
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    chkstr9.push_back('n');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    if(res == true) cout << "test 9 passed" << endl; else cout << "test 9 FAILURE" << endl;

    chkstr9.clear();
    chkstr9.push_back('a');
    chkstr9.push_back('a');
    chkstr9.push_back('a');
    res = s9.exists(chkstr9);
    if(res == false) cout << "test 9 passed" << endl; else cout << "test 9 FAILURE" << endl;
  }


}
