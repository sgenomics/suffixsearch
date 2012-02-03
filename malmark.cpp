#include <vector>
#include <algorithm>
#include <iostream>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include "tialloc.h"


using namespace std;

int mallocmark() {

  time_t start_time = time(NULL);

  srand(0);

  for(size_t i=0;i<1000;i++) {
    vector<void *> allocs;
    size_t total_size = 0;
    for(size_t n=0;n<100000;n++) {

      void *a = malloc((rand()%500)+1);

      allocs.push_back(a);
      ((char *)a)[0] = 4;

      // total_size += malloc_usable_size(a);
    }

    random_shuffle(allocs.begin(),allocs.end());

    for(size_t n=0;n<allocs.size();n++) {
      free(allocs[n]);
    }
  }


  time_t end_time = time(NULL);

  cout << "malloc time: " << end_time - start_time << " seconds" <<  endl;
}

int tiallocmark() {

  void *a = tialloc::instance()->alloc((rand()%500)+1);
  tialloc::instance()->free(a);
  time_t start_time = time(NULL);

  srand(0);

  for(size_t i=0;i<1000;i++) {
    vector<void *> allocs;
    size_t total_size = 0;
    for(size_t n=0;n<100000;n++) {

      void *a = tialloc::instance()->alloc((rand()%500)+1);

      allocs.push_back(a);
      ((char *)a)[0] = 4;

      // total_size += tialloc::instance()->alloc_size(a);
    }

    random_shuffle(allocs.begin(),allocs.end());

    for(size_t n=0;n<allocs.size();n++) {
      tialloc::instance()->free(allocs[n]);
    }
  }

  time_t end_time = time(NULL);

  cout << "tialloc time: " << end_time - start_time << " seconds" <<  endl;
}


int main() {
  mallocmark();
  tiallocmark();
}
