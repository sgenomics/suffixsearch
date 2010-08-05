#ifndef SEARCHSTORE
#define SEARCHSTORE

#include <vector>

using namespace std;

class SearchStore {

public:
  SearchStore() {
    
  } 

  virtual bool exists(vector<char> t) = 0;

  virtual void insert(char current_symbol) = 0;
};

#endif
