#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"
#include <fstream>
#include <time.h>
#include "stringify.h"

using namespace std;

class SeedVertex {

public:

  SeedVertex(int position_in,int number_in) : position(position_in), number(number_in) {}

  int position;
  int number;
};

bool consistant_path(vector<vector<int> > &seed_positions,SeedVertex v1,SeedVertex v2,int spacing) {
  int seed1_position = seed_positions[v1.position][v1.number];
  int seed2_position = seed_positions[v2.position][v2.number];

  int seed_distance = (v2.number - v1.number) * spacing;

  if(((seed1_position + seed_distance) - seed2_position) < 8) return true;

  return false;
}

bool location_finder(vector<vector<int> > seed_positions,size_t location,size_t score,int spacing) {

  // this method finds consistant regions in seed_positions
  vector<vector<SeedVertex> > paths;
  for(size_t n=0;n<seed_positions[0].size();n++) {
    vector<SeedVertex> s;
    s.push_back(SeedVertex(0,n));
    paths.push_back(s);
  }

  // build seed paths
  int current_level = 1;
  for(size_t current_level=1;current_level<seed_positions.size();current_level++) {
    for(size_t n=0;n<seed_positions[current_level].size();n++) {
      bool found=false;
      for(size_t i=0;i<paths.size();i++) {
        if(consistant_path(seed_positions,paths[i][paths[i].size()-1],SeedVertex(current_level,n),spacing)) {
          paths[i].push_back(SeedVertex(n,i));
          found=true;
          break;
        }
      }
      if(!found) {
        vector<SeedVertex> v;
        v.push_back(SeedVertex(current_level,n));
        paths.push_back(v);
      }
    }
  }

  // find the longest path
  int max_path_idx=0;
  int max_path_len=0;
  for(size_t n=0;n<paths.size();n++) {
    if(paths[n].size() > max_path_len) { max_path_len = paths[n].size(); max_path_idx = n; }
  }

  return seed_positions[paths[max_path_idx][0].position][paths[max_path_idx][0].number];
}

vector<char> simulate_read(int read_length,int sub_rate,int ins_rate,int del_rate,SuffixTree &st,size_t &location) {

  size_t seq_size = st.s.size();

  // extract read_length location from read
  size_t position = rand()%(seq_size-read_length);
  location = position;
  string ss = st.get_substr(position,position+read_length);

 // cout << "Read: " << ss << endl;

  vector<char> s;
  for(size_t n=0;n<ss.size();n++) s.push_back(ss[n]);

  // Apply substition errors
  for(size_t n=0;n<s.size();n++) {
    if(((rand()%1000)/10) < sub_rate) s[n] = stringify(rand()%10)[0];
  }

  return s;
}

int main(int argc,char ** argv) {

  string s = argv[1];

  ifstream infile(argv[1]);


  SuffixTree st;

  size_t seq_size=0;
  for(;!infile.eof();) {
    char c;
    infile.get(c);
    st.insert(c);
    seq_size++;
  }

  st.finalise();
  cout << "built suffix tree" << endl;

  cout << "processing positions" << endl;
  st.process_positions();
  cout << "processed positions" << endl;
  st.dump_stats();

  time_t t1,t2;
  time(&t1);
  for(size_t n=0;n<200000;n++) {

    // Simulate read
    size_t location;
    vector<char> read = simulate_read(100,0.1,0.1,0.1,st,location);
    cout << location << endl;

    // extract seed sequences
    int seed_count=40;
    int seed_size =9;
    int seed_dist=read.size()/seed_count;

    vector<vector<char> > seeds;
    for(size_t i=0;i<seed_count;i++) {
      vector<char> seed;
      for(size_t j=((seed_dist)*i);j<(((seed_dist)*i)+10);j++) {
        seed.push_back(read[j]);
      }

      seeds.push_back(seed);
    }

    bool debug=false;

    // Align seed sequences
    vector<vector<int> > seed_positions;
    for(size_t i=0;i<seeds.size();i++) {
      vector<int> occurs = st.all_occurs(seeds[i],8);
      seed_positions.push_back(occurs);
      if(debug) {
        if(occurs.size() > 8) cout << "- "; else cout << occurs.size() << " ";
      }
    }

    if(debug) cout<< endl;

    if(debug) {
      for(size_t i=0;i<seed_positions.size();i++) {
        if(seed_positions[i].size() > 8) {cout << "-";} else {
          for(size_t j=0;j<seed_positions[i].size();j++) {
            cout << seed_positions[i][j] << " ";
          }
        }
        cout << endl;
      }
    }

    // Find consistant seeds
    //size_t align_location;
    //size_t align_location_score;
    //location_finder(seed_positions,align_location,align_location_score,seed_dist);
    //cout << location << " " << align_location << " " << align_location_score << endl;

  }

  time(&t2);
  cout << "Time: " << t2-t1 << endl; 

}
