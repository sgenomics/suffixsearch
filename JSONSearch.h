#ifndef JSONSEARCH
#define JSONSEARCH

#include <vector>

using namespace std;

class JSONSearch {

public:
  JSONSearch(SearchStore &store) : m_store(store) {
    
  } 

  string process_command(string json_command) {
  }

  SearchStore &m_store;
};

#endif
