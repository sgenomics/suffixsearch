#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"

using namespace std;


int main() {

  SuffixTree s;

  s.insert('b');
  s.insert('a');
  s.insert('n');
  s.insert('a');
  s.insert('n');
  s.insert('a');
  s.insert('$');

  s.dump();

  // simple existance tests

  vector<char> chkstr;
  chkstr.push_back('a');
  chkstr.push_back('n');
  chkstr.push_back('a');
  bool res;

  res = s.exists(chkstr);
  if(res == true) cout << "test 1 passed" << endl; else cout << "test 1 failed" << endl;

  chkstr.clear();
  chkstr.push_back('a');
  chkstr.push_back('a');
  chkstr.push_back('a');
  res = s.exists(chkstr);
  if(res == false) cout << "test 2 passed" << endl; else cout << "test 2 failed" << endl;
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

  for(int i=0;i<500;i++) {
    vector<char> str2;
    vector<char> chkstr2;

    cout << "randomised string: ";
    for(int n=0;n<20;n++) {
      char c = (rand()%3)+65;
      str2.push_back(c);
      cout << c;
    }
    str2.push_back('$');
    cout << endl;

    cout << "check string: ";
    for(int n=0+(rand()%2);n<(6+rand()%1);n++) {
      chkstr2.push_back(str2[n]);
      cout << str2[n];
    }
    cout << endl;
    
    SuffixTree s2;
    for(size_t n=0;n<str2.size();n++) {
      s2.insert(str2[n]);
    }

    res = s2.exists(chkstr2);
    cout << "rand str: "; for(size_t n=0;n<str2.size();n++) cout << str2[n]; cout << endl;
    cout << "chk  str: "; for(size_t n=0;n<chkstr2.size();n++) cout << chkstr2[n]; cout << endl;
    if(res == true) cout << "rand 1 passed" << endl; else cout << "rand 1 failed" << endl;

    vector<char> chkstr3;
    chkstr3.push_back('z');
    chkstr3.push_back('z');
    chkstr3.push_back('z');
    res = s2.exists(chkstr3);
    if(res == false) cout << "rand 2 passed" << endl; else cout << "rand 2 failed" << endl;

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
    cout << "rand str: "; for(size_t n=0;n<str2.size();n++) cout << str2[n]; cout << endl;
    cout << "s2str: *" << s2str << "*" << endl;
    if(findit == false) cout << "shouldn't find it" << endl;
    if(findit == true ) cout << "should    find it" << endl;
    if(res == findit) cout << "rand 3 passed" << endl; else cout << "rand 3 failed" << endl;

  }

}

