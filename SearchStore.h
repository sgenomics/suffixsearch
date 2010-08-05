#ifndef SEARCHSTORE
#define SEARCHSTORE

#include <vector>

using namespace std;

class SearchStore {

public:
  SearchStore() {
    
  } 

  virtual bool exists(vector<char> t);

  virtual void insert(char current_symbol);
};

#endif
