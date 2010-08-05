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
  seconds1 = time (NULL);
  for(int n=0;n<ms.size();n++) {
    s1.insert(ms[n]);
  }
  seconds2 = time (NULL);

  return seconds2-seconds1;
}

int test_suffixtree_time(UnitTest &utf) {

  utf.begin_test_set("Timing SuffixTree tests");

  long int tree_time200000  = make_tree(200000); 
  long int tree_time300000  = make_tree(300000); 
  long int tree_time400000  = make_tree(400000); 
  long int tree_time500000  = make_tree(500000); 
  long int tree_time600000  = make_tree(600000); 
  long int tree_time800000  = make_tree(800000); 
  long int tree_time1000000 = make_tree(1000000); 

  utf.test_truth(tree_time400000  <= (tree_time200000*2)+1);
  utf.test_truth(tree_time600000  <= (tree_time300000*2)+1);
  utf.test_truth(tree_time800000  <= (tree_time400000*2)+1);
  utf.test_truth(tree_time1000000 <= (tree_time500000*2)+1);

  utf.end_test_set();

}
