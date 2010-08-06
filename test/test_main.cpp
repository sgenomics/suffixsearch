#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "UnitTest.h"
#include "test_suffixtree_simple.h"
#include "test_suffixtree_random.h"
#include "test_suffixtree_time.h"
#include "test_suffixtree_processpositions.h"

using namespace std;

int main(int argc,char ** argv) {
  UnitTest utf("41J Tests");

  test_suffixtree_simple(utf);
  test_suffixtree_random(utf);
  test_suffixtree_time(utf);
  test_suffixtree_processpositions(utf);
  utf.test_report();
}
