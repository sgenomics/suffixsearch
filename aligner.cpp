#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "SuffixTree.h"
#include <fstream>
#include <time.h>
#include "stringify.h"

#define NULLTRANSCODE

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

bool location_finder_pile(vector<vector<int> > seed_positions,size_t &location,size_t &score,int spacing) {

  vector<int> all_pos;
  for(size_t n=0;n<seed_positions.size();n++) {
//    if(seed_positions[n].size()<100) {
      for(size_t m=0;m<seed_positions[n].size();m++) {
        all_pos.push_back(seed_positions[n][m]);
      }
//    }
  }

  if(all_pos.size() == 0) {
    location = 999999;
    score    = 0;
    return false;
  }
  //sort(all_pos.begin(),all_pos.end());
 
  vector<int> bin_ids;
  vector<int> bin_count;

  for(size_t n=0;n<all_pos.size();n++) {
    bool found=false;
    for(size_t i=0;i<bin_ids.size();i++) {
      int delta = bin_ids[i]-all_pos[n];
      if(delta < 0) delta = 0-delta;
      if(delta < 100) {found=true; bin_count[i]++; break;}
    }
    if(found==false) {
      bin_ids.push_back(all_pos[n]);
      bin_count.push_back(0);
    }
  }


  // find largest bin
  int max = 0;
  int maxid=0;
  for(size_t n=0;n<bin_ids.size();n++) {
    if(bin_count[n] > max) { max = bin_count[n]; maxid = n; }
  }

  // find all values near maxid
  int good_pos = bin_ids[maxid];
  long int all_good=0;
  long int num_good=0;
  for(size_t n=0;n<all_pos.size();n++) {
    int delta = all_pos[n]-good_pos;
    if(delta < 0) delta = 0-delta;

    if(delta < 200) {all_good += all_pos[n]; num_good++;}
  }

  location = (size_t) all_good/num_good;
  score    = (size_t) ((((double)num_good)/((double)all_pos.size()))*100000);
  return true;
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
  infile.close();

  st.finalise();
  cout << "built suffix tree" << endl;

  cout << "processing positions" << endl;
  st.process_positions();
  cout << "processed positions" << endl;
  st.dump_stats();

  // load reads
  ifstream readfile(argv[2]);
  vector<string> reads;
  for(;!readfile.eof();) {
    string read;
    readfile >> read;
    reads.push_back(read);
  }

  int seed_size = convertTo<int>(argv[3]);

  time_t t1,t2;
  time(&t1);
  for(size_t n=0;n<reads.size();n++) {

    vector<char> read;
    for(size_t i=0;i<reads[n].size();i++) {read.push_back(reads[n][i]);}

    // extract seed sequences
    int seed_dist = 1;
    bool debug=false;

    vector<vector<char> > seeds;
    if(debug) cout << "read size: " << read.size() << endl;
    if(read.size() > seed_size) {
      for(size_t seed_start=0;seed_start<(read.size()-seed_size);seed_start+=seed_dist) {
	vector<char> seed;
	for(size_t j=seed_start;j<(seed_start+seed_size);j++) {
	  seed.push_back(read[j]);
	}

	seeds.push_back(seed);
      }
      if(debug) cout << "seed count: " << seeds.size() << endl;


      // Align seed sequences
      vector<vector<int> > seed_positions;
      for(size_t i=0;i<seeds.size();i++) {
	vector<int> occurs = st.all_occurs(seeds[i]); //set second arg
	seed_positions.push_back(occurs);
	if(debug) {
	  if(occurs.size() > 9) cout << "- "; else cout << occurs.size() << " ";
	}
      }

      if(debug) cout<< endl;

      if(debug) {
	for(size_t i=0;i<seed_positions.size();i++) {
	  if(seed_positions[i].size() > 200) {cout << "-";} else {
	    for(size_t j=0;j<seed_positions[i].size();j++) {
	      cout << seed_positions[i][j] << " ";
//	      if(seed_positions[i][j] < 10000) cout << seed_positions[i][j] << " "; else cout << "= ";
	    }
	  }
	  cout << endl;
	}
      }

      //Find consistant seeds
      size_t align_location;
      size_t align_location_score;
      location_finder_pile(seed_positions,align_location,align_location_score,seed_dist);
      //location_finder(seed_positions,align_location,align_location_score,seed_dist);
      cout << "Alignment: " << align_location << " " << align_location_score << endl;

    }
  }

  time(&t2);
  cout << "Time: " << t2-t1 << endl; 

}
