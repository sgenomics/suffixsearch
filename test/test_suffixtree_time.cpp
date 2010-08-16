#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"
#include "UnitTest.h"
#include <time.h>

using namespace std;

long int make_tree(long int size) {
  string ms;
  SuffixTree s1;

  // fill ms with size random characters
  for(long int n=0;n<size;n++) {
    int r = rand()%4;
    if(r == 0) ms.push_back('a');
    if(r == 1) ms.push_back('b');
    if(r == 2) ms.push_back('c');
    if(r == 3) ms.push_back('d');
  }

  time_t seconds1;
  time_t seconds2;
  seconds1 = clock ();
  for(int n=0;n<ms.size();n++) {
    s1.insert(ms[n]);
  }
  seconds2 = clock ();

  return seconds2-seconds1;
}

int test_suffixtree_time(UnitTest &utf) {

  utf.begin_test_set("Timing SuffixTree tests");

  long int tree_time100000 = make_tree(100000); 
  long int tree_time200000 = make_tree(200000); 
  cout << "100000 time: " << tree_time100000 << endl;
  cout << "200000 time: " << tree_time200000 << endl;
  utf.test_truth(tree_time200000 <= (tree_time100000*2)+60000);

  utf.end_test_set();
}
