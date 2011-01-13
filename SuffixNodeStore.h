#ifndef SUFFIXNODESTORE
#define SUFFIXNODESTORE

#include "global_defs.h"
#include <vector>
#include <map>
#include <algorithm>
#include "ObjectStoreMem.h"
#include "ObjectStoreMemVec.h"
#include "ObjectStoreDisk.h"
#include "global_defs.h"

using namespace std;

#include "SuffixNode.h"

#include <stdint.h>

template<class v_type>
bool isinvalid(v_type &v) {

  return !v.isvalid();
}


template<class vec_type>
size_t get_next_invalid(vec_type &v,size_t position) {

  for(size_t n=position;n<v.size();n++) {
    if(!v.get(n).isvalid()) return n;
  }

  return 0;

}

template<class vec_type>
size_t get_last_valid(vec_type &v,size_t position) {

  for(size_t n=position;n>=0;n++) {
    if(v.get(n).isvalid()) return n;
  }

  return 0;
}


template<class vec_type>
void compact_vec(vec_type &v,map<int32_t,int32_t> &mapping,int32_t id) {
/*
  // version that replaces object store
  vec_type new_v;

  cout << "pre: " << v.size() << endl;
  for(size_t n=0;n<v.size();n++) {
    if(v.get(n).isvalid()) {
      size_t nid = new_v.push_back(v.get(n));
      mapping[n+id] = nid + id;
    }
  }

  cout << "post: " << new_v.size() << " removed: " << v.size()-new_v.size() << endl;
  v = new_v;
*/

cout << "compact code called" << endl;
  size_t last_invalid = 0;
  size_t last_valid   = v.size()-1;

  size_t invalid_count = 0;
  for(size_t n=0;n<v.size();n++) {
    size_t i = get_next_invalid(v,last_invalid);
    size_t j = get_last_valid  (v,last_valid);

    last_invalid = i;
    last_valid   = j-1;

    if(i == 0) break;
    if(j == 0) break;

    v.set(i,v.get(j));
    mapping[j+id] = i+id;
    invalid_count++;
  }

  v.current_max = v.current_max-invalid_count;

/*

  // version that updates existing object store.
  if(v.size() > 2) {
    size_t old_size = v.size();
    cout << "pre compact child store size: " << v.size();

    int32_t lastvalid = -1;
    for(int32_t n=0;n< static_cast<int32_t>(v.size());n++) {

      if(v.get(n).isvalid() == false) {
        if(lastvalid != -1) {
          v.set(lastvalid,v.get(n));
          mapping[n+id] = lastvalid+id;
        } else {
          mapping[n+id] = 0+id; // WAS just line below
          lastvalid = 0;
        }

      } else {
	if(lastvalid != n) {
	  if(lastvalid != -1) { v.set(lastvalid,v.get(n)); } else lastvalid = 0;
//	  mapping[n+id] = lastvalid+id;
	}
	mapping[n+id] = lastvalid+id;
	lastvalid++;
      }
    }

    v.current_max = lastvalid;
    cout << "post: " << v.size() << " removed: " << old_size-v.size() << endl;
  }
*/

}

template<class vec_type>
void apply_mapping(vec_type &v, map<int32_t,int32_t> &mapping) {

  for(size_t n=0;n<v.size();n++) {
    if(v.get(n).childlist_idx != -1) {
      typename vec_type::value_type snode = v.get(n);
      
      map<int32_t,int32_t>::iterator i = mapping.find(v.get(n).childlist_idx);

      if(i == mapping.end()) {
   //     this is fine.
   //     cout << "error no mapping for: " << v.get(n).childlist_idx << endl;
      } else {
        snode.childlist_idx = mapping[v.get(n).childlist_idx];
        v.set(n,snode);
      }
    }
  }
}


template<int childcount>
class ChildList {
  public:

    ChildList(int d=-1) {
      for(size_t n=0;n<childcount;n++) {
        symbol[n] = d;
        index [n] = d;
      }
    }

    vector<SymbolPair> get_childlist() {

      vector<SymbolPair> p;

      for(size_t n=0;n<childcount;n++) {
        p.push_back(SymbolPair(symbol[n],index[n]));
      }

      return p;
    }

    ChildList<childcount> &set_children(vector<SymbolPair> &s) {
      if(s.size() != childcount) {cout << "EEEEERRRRORRRRRR" << endl; exit(0);}
      for(size_t n=0;n<childcount;n++) {
        symbol[n] = s[n].symbol;
        index [n] = s[n].index;
        //cout << "childlist set: " << static_cast<int>(symbol[n]) << "," << index[n] << endl;
      }
      return *this;
    }

    bool isvalid() {
      for(size_t n=0;n<childcount;n++) {if (index[n] != -1) return true;}
      return false;
    }

    bool operator==(ChildList<childcount> &other) {
      return equal(other);
    }

    bool equal(ChildList<childcount> &other) {
      for(size_t n=0;n<childcount;n++) {
        if(symbol[n] != other.symbol[n]) { return false; }
        if(index [n] != other.index [n]) { return false; }
      }
      return true;
    }

    int8_t  symbol[childcount];
    int32_t index [childcount];
} __attribute__((__packed__));

//template<template<class T> class object_store_type=ObjectStoreMem>
class ChildListStore {

public:

  ChildListStore() {
  //  cout << "creating a ChildListStore" << endl;
  }

  vector<SymbolPair> get_children(int32_t idx) {
//cout << "CHILD LIST STORE GET" << endl;
//    cout << "getting children id: " << idx << endl;

    int id = get_store_id(idx);

//cout << "fetching id: " << id << endl;
//cout << "fetching idx: " << (0x00FFFFFF & idx) << endl;
//    cout << " id is: "  << id << endl;
    if(id == 0) {cout << "GETerror nodes with 0   child should not be possible" << endl; int *i=0;*i=1; }
    if(id == 1) {cout << "GETerror nodes with one child should not be possible" << endl; int *i=0;*i=1; }
    if(id == 2 ) return children_2 .get(0x00FFFFFF & idx).get_childlist();
    if(id == 3 ) return children_3 .get(0x00FFFFFF & idx).get_childlist();
    if(id == 4 ) return children_4 .get(0x00FFFFFF & idx).get_childlist();
    if(id == 5 ) return children_5 .get(0x00FFFFFF & idx).get_childlist();
    if(id == 6 ) return children_6 .get(0x00FFFFFF & idx).get_childlist();
    if(id == 7 ) return children_7 .get(0x00FFFFFF & idx).get_childlist();
    if(id == 8 ) return children_8 .get(0x00FFFFFF & idx).get_childlist();
    if(id == 9 ) return children_9 .get(0x00FFFFFF & idx).get_childlist();
    if(id == 10) return children_10.get(0x00FFFFFF & idx).get_childlist();
    if(id == 11) return children_11.get(0x00FFFFFF & idx).get_childlist();
    if(id == 12) return children_12.get(0x00FFFFFF & idx).get_childlist();
    if(id == 13) return children_13.get(0x00FFFFFF & idx).get_childlist();
    if(id == 14) return children_14.get(0x00FFFFFF & idx).get_childlist();
    if(id == 15) return children_15.get(0x00FFFFFF & idx).get_childlist();
    if(id == 16) return children_16.get(0x00FFFFFF & idx).get_childlist();
    if(id == 17) return children_17.get(0x00FFFFFF & idx).get_childlist();
    if(id == 18) return children_18.get(0x00FFFFFF & idx).get_childlist();
    if(id == 19) return children_19.get(0x00FFFFFF & idx).get_childlist();
    if(id == 20) return children_20.get(0x00FFFFFF & idx).get_childlist();
    if(id == 21) return children_21.get(0x00FFFFFF & idx).get_childlist();
    if(id == 22) return children_22.get(0x00FFFFFF & idx).get_childlist();
    if(id == 23) return children_23.get(0x00FFFFFF & idx).get_childlist();
    if(id == 24) return children_24.get(0x00FFFFFF & idx).get_childlist();
    if(id == 25) return children_25.get(0x00FFFFFF & idx).get_childlist();
    if(id == 26) return children_26.get(0x00FFFFFF & idx).get_childlist();
    if(id == 27) return children_27.get(0x00FFFFFF & idx).get_childlist();
    if(id == 28) return children_28.get(0x00FFFFFF & idx).get_childlist();
    if(id == 29) return children_29.get(0x00FFFFFF & idx).get_childlist();
    if(id == 30) return children_30.get(0x00FFFFFF & idx).get_childlist();
    if(id == 31) return children_31.get(0x00FFFFFF & idx).get_childlist();
    if(id == 32) return children_32.get(0x00FFFFFF & idx).get_childlist();
    if(id == 33) return children_33.get(0x00FFFFFF & idx).get_childlist();
    if(id == 34) return children_34.get(0x00FFFFFF & idx).get_childlist();
    if(id == 35) return children_35.get(0x00FFFFFF & idx).get_childlist();
    if(id == 36) return children_36.get(0x00FFFFFF & idx).get_childlist();
    if(id == 37) return children_37.get(0x00FFFFFF & idx).get_childlist();
    if(id == 38) return children_38.get(0x00FFFFFF & idx).get_childlist();
    if(id == 39) return children_39.get(0x00FFFFFF & idx).get_childlist();
    if(id == 40) return children_40.get(0x00FFFFFF & idx).get_childlist();
    cout << "1error invalid id" << endl; children_2.set(10000000,-1);
    return vector<SymbolPair>();
  }

  void set_children(int32_t idx,vector<SymbolPair> &p) {

//    cout << "setting children id: " << idx << endl;

    int32_t id = get_store_id(idx);
    if(id != static_cast<int32_t>(p.size())) { cout << "ERROR: new size does not equal old size cannot use same idx" << endl; return; }

    if(id == 1) {cout << "error nodes with one child should not be possible" << endl; children_2.set(10000000,-1); } else
    if(id == 2) { for(size_t i=0;i<p.size();i++) { ChildList<2> original = children_2 .get(idx-0x02000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_2 .set(idx-0x02000000,original); } } else
    if(id == 3) { for(size_t i=0;i<p.size();i++) { ChildList<3> original = children_3 .get(idx-0x03000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_3 .set(idx-0x03000000,original); } } else
    if(id == 4) { for(size_t i=0;i<p.size();i++) { ChildList<4> original = children_4 .get(idx-0x04000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_4 .set(idx-0x04000000,original); } } else
    if(id == 5) { for(size_t i=0;i<p.size();i++) { ChildList<5> original = children_5 .get(idx-0x05000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_5 .set(idx-0x05000000,original); } } else
    if(id == 6) { for(size_t i=0;i<p.size();i++) { ChildList<6> original = children_6 .get(idx-0x06000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_6 .set(idx-0x06000000,original); } } else
    if(id == 7) { for(size_t i=0;i<p.size();i++) { ChildList<7> original = children_7 .get(idx-0x07000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_7 .set(idx-0x07000000,original); } } else
    if(id == 8) { for(size_t i=0;i<p.size();i++) { ChildList<8> original = children_8 .get(idx-0x08000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_8 .set(idx-0x08000000,original); } } else
    if(id == 9) { for(size_t i=0;i<p.size();i++) { ChildList<9> original = children_9 .get(idx-0x09000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_9 .set(idx-0x09000000,original); } } else
    if(id ==10) { for(size_t i=0;i<p.size();i++) { ChildList<10>original = children_10.get(idx-0x0A000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_10.set(idx-0x0A000000,original); } } else
    if(id ==11) { for(size_t i=0;i<p.size();i++) { ChildList<11>original = children_11.get(idx-0x0B000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_11.set(idx-0x0B000000,original); } } else
    if(id ==12) { for(size_t i=0;i<p.size();i++) { ChildList<12>original = children_12.get(idx-0x0C000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_12.set(idx-0x0C000000,original); } } else
    if(id ==13) { for(size_t i=0;i<p.size();i++) { ChildList<13>original = children_13.get(idx-0x0D000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_13.set(idx-0x0D000000,original); } } else
    if(id ==14) { for(size_t i=0;i<p.size();i++) { ChildList<14>original = children_14.get(idx-0x0E000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_14.set(idx-0x0E000000,original); } } else
    if(id ==15) { for(size_t i=0;i<p.size();i++) { ChildList<15>original = children_15.get(idx-0x0F000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_15.set(idx-0x0F000000,original); } } else
    if(id ==16) { for(size_t i=0;i<p.size();i++) { ChildList<16>original = children_16.get(idx-0x10000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_16.set(idx-0x10000000,original); } } else
    if(id ==17) { for(size_t i=0;i<p.size();i++) { ChildList<17>original = children_17.get(idx-0x11000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_17.set(idx-0x11000000,original); } } else
    if(id ==18) { for(size_t i=0;i<p.size();i++) { ChildList<18>original = children_18.get(idx-0x12000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_18.set(idx-0x12000000,original); } } else
    if(id ==19) { for(size_t i=0;i<p.size();i++) { ChildList<19>original = children_19.get(idx-0x13000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_19.set(idx-0x13000000,original); } } else
    if(id ==20) { for(size_t i=0;i<p.size();i++) { ChildList<20>original = children_20.get(idx-0x14000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_20.set(idx-0x14000000,original); } } else
    if(id ==21) { for(size_t i=0;i<p.size();i++) { ChildList<21>original = children_21.get(idx-0x15000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_21.set(idx-0x15000000,original); } } else
    if(id ==22) { for(size_t i=0;i<p.size();i++) { ChildList<22>original = children_22.get(idx-0x16000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_22.set(idx-0x16000000,original); } } else
    if(id ==23) { for(size_t i=0;i<p.size();i++) { ChildList<23>original = children_23.get(idx-0x17000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_23.set(idx-0x17000000,original); } } else
    if(id ==24) { for(size_t i=0;i<p.size();i++) { ChildList<24>original = children_24.get(idx-0x18000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_24.set(idx-0x18000000,original); } } else
    if(id ==25) { for(size_t i=0;i<p.size();i++) { ChildList<25>original = children_25.get(idx-0x19000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_25.set(idx-0x19000000,original); } } else
    if(id ==26) { for(size_t i=0;i<p.size();i++) { ChildList<26>original = children_26.get(idx-0x1A000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_26.set(idx-0x1A000000,original); } } else
    if(id ==27) { for(size_t i=0;i<p.size();i++) { ChildList<27>original = children_27.get(idx-0x1B000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_27.set(idx-0x1B000000,original); } } else
    if(id ==28) { for(size_t i=0;i<p.size();i++) { ChildList<28>original = children_28.get(idx-0x1C000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_28.set(idx-0x1C000000,original); } } else
    if(id ==29) { for(size_t i=0;i<p.size();i++) { ChildList<29>original = children_29.get(idx-0x1D000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_29.set(idx-0x1D000000,original); } } else
    if(id ==30) { for(size_t i=0;i<p.size();i++) { ChildList<30>original = children_30.get(idx-0x1E000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_30.set(idx-0x1E000000,original); } } else
    if(id ==31) { for(size_t i=0;i<p.size();i++) { ChildList<31>original = children_31.get(idx-0x1F000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_31.set(idx-0x1F000000,original); } } else
    if(id ==32) { for(size_t i=0;i<p.size();i++) { ChildList<32>original = children_32.get(idx-0x20000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_32.set(idx-0x20000000,original); } } else
    if(id ==33) { for(size_t i=0;i<p.size();i++) { ChildList<33>original = children_33.get(idx-0x21000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_33.set(idx-0x21000000,original); } } else
    if(id ==34) { for(size_t i=0;i<p.size();i++) { ChildList<34>original = children_34.get(idx-0x22000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_34.set(idx-0x22000000,original); } } else
    if(id ==35) { for(size_t i=0;i<p.size();i++) { ChildList<35>original = children_35.get(idx-0x23000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_35.set(idx-0x23000000,original); } } else
    if(id ==36) { for(size_t i=0;i<p.size();i++) { ChildList<36>original = children_36.get(idx-0x24000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_36.set(idx-0x24000000,original); } } else
    if(id ==37) { for(size_t i=0;i<p.size();i++) { ChildList<37>original = children_37.get(idx-0x25000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_37.set(idx-0x25000000,original); } } else
    if(id ==38) { for(size_t i=0;i<p.size();i++) { ChildList<38>original = children_38.get(idx-0x26000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_38.set(idx-0x26000000,original); } } else
    if(id ==39) { for(size_t i=0;i<p.size();i++) { ChildList<39>original = children_39.get(idx-0x27000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_39.set(idx-0x27000000,original); } } else
    if(id ==40) { for(size_t i=0;i<p.size();i++) { ChildList<40>original = children_40.get(idx-0x28000000); original.symbol[i] = p[i].symbol; original.index [i] = p[i].index; children_40.set(idx-0x28000000,original); } } else
    { cout << "2error invalid id" << endl; children_2.set(10000000,-1); }

  }

  int32_t push_back(vector<SymbolPair> &p) {

 /*   cout << "child store pushing this size: " << p.size() << endl;
    for(size_t n=0;n<p.size();n++) {
      cout << static_cast<int>(p[n].symbol);
      cout << ",";
      cout << p[n].index << endl;
    }
    cout << "lend" << endl;
*/
    // 1. find our which storage section to put it in.
    int id = p.size();
    if(id == 1) {cout << "PUSHerror nodes with one child should not be possible" << endl; children_2.set(10000000,-1); return -1;}
    // 2. put it there
    // 3. return the size()-1
    if(id == 2) {//cout << "push2" << endl;
      ChildList<2 >c;
      c.set_children(p);
      int id0 = children_2 .push_back(c);

      return 0x02000000+id0;
    } else
    if(id == 3) {//cout << "push3" << endl;
      ChildList<3 >c;
      c.set_children(p);
      int id0 = children_3 .push_back(c);

     // cout << "THE ID IS: " << id0 << endl;
      ChildList<3> c2 = children_3.get(id0);
      vector<SymbolPair> p2 = c2.get_childlist();
     // for(size_t n=0;n<p2.size();n++) {
      //  cout << static_cast<int>(p2[n].symbol) << " ; " << p2[n].index << endl;
     // }
     // cout << "lend3" << endl;
//cout << "return now" << endl;
      return 0x03000000+id0;
    } else 
    if(id == 4) {int32_t nid = children_4 .push_back(ChildList<4 >().set_children(p)); return 0x04000000+nid;} else
    if(id == 5) {int32_t nid = children_5 .push_back(ChildList<5 >().set_children(p)); return 0x05000000+nid;} else
    if(id == 6) {int32_t nid = children_6 .push_back(ChildList<6 >().set_children(p)); return 0x06000000+nid;} else
    if(id == 7) {int32_t nid = children_7 .push_back(ChildList<7 >().set_children(p)); return 0x07000000+nid;} else
    if(id == 8) {int32_t nid = children_8 .push_back(ChildList<8 >().set_children(p)); return 0x08000000+nid;} else
    if(id == 9) {int32_t nid = children_9 .push_back(ChildList<9 >().set_children(p)); return 0x09000000+nid;} else
    if(id ==10) {int32_t nid = children_10.push_back(ChildList<10>().set_children(p)); return 0x0A000000+nid;} else
    if(id ==11) {int32_t nid = children_11.push_back(ChildList<11>().set_children(p)); return 0x0B000000+nid;} else
    if(id ==12) {int32_t nid = children_12.push_back(ChildList<12>().set_children(p)); return 0x0C000000+nid;} else
    if(id ==13) {int32_t nid = children_13.push_back(ChildList<13>().set_children(p)); return 0x0D000000+nid;} else
    if(id ==14) {int32_t nid = children_14.push_back(ChildList<14>().set_children(p)); return 0x0E000000+nid;} else
    if(id ==15) {int32_t nid = children_15.push_back(ChildList<15>().set_children(p)); return 0x0F000000+nid;} else
    if(id ==16) {int32_t nid = children_16.push_back(ChildList<16>().set_children(p)); return 0x10000000+nid;} else
    if(id ==17) {int32_t nid = children_17.push_back(ChildList<17>().set_children(p)); return 0x11000000+nid;} else
    if(id ==18) {int32_t nid = children_18.push_back(ChildList<18>().set_children(p)); return 0x12000000+nid;} else
    if(id ==19) {int32_t nid = children_19.push_back(ChildList<19>().set_children(p)); return 0x13000000+nid;} else
    if(id ==20) {int32_t nid = children_20.push_back(ChildList<20>().set_children(p)); return 0x14000000+nid;} else
    if(id ==21) {int32_t nid = children_21.push_back(ChildList<21>().set_children(p)); return 0x15000000+nid;} else
    if(id ==22) {int32_t nid = children_22.push_back(ChildList<22>().set_children(p)); return 0x16000000+nid;} else
    if(id ==23) {int32_t nid = children_23.push_back(ChildList<23>().set_children(p)); return 0x17000000+nid;} else
    if(id ==24) {int32_t nid = children_24.push_back(ChildList<24>().set_children(p)); return 0x18000000+nid;} else
    if(id ==25) {int32_t nid = children_25.push_back(ChildList<25>().set_children(p)); return 0x19000000+nid;} else
    if(id ==26) {int32_t nid = children_26.push_back(ChildList<26>().set_children(p)); return 0x1A000000+nid;} else
    if(id ==27) {int32_t nid = children_27.push_back(ChildList<27>().set_children(p)); return 0x1B000000+nid;} else
    if(id ==28) {int32_t nid = children_28.push_back(ChildList<28>().set_children(p)); return 0x1C000000+nid;} else
    if(id ==29) {int32_t nid = children_29.push_back(ChildList<29>().set_children(p)); return 0x1D000000+nid;} else
    if(id ==30) {int32_t nid = children_30.push_back(ChildList<30>().set_children(p)); return 0x1E000000+nid;} else
    if(id ==31) {int32_t nid = children_31.push_back(ChildList<31>().set_children(p)); return 0x1F000000+nid;} else
    if(id ==32) {int32_t nid = children_32.push_back(ChildList<32>().set_children(p)); return 0x20000000+nid;} else
    if(id ==33) {int32_t nid = children_33.push_back(ChildList<33>().set_children(p)); return 0x21000000+nid;} else
    if(id ==34) {int32_t nid = children_34.push_back(ChildList<34>().set_children(p)); return 0x22000000+nid;} else
    if(id ==35) {int32_t nid = children_35.push_back(ChildList<35>().set_children(p)); return 0x23000000+nid;} else
    if(id ==36) {int32_t nid = children_36.push_back(ChildList<36>().set_children(p)); return 0x24000000+nid;} else
    if(id ==37) {int32_t nid = children_37.push_back(ChildList<37>().set_children(p)); return 0x25000000+nid;} else
    if(id ==38) {int32_t nid = children_38.push_back(ChildList<38>().set_children(p)); return 0x26000000+nid;} else
    if(id ==39) {int32_t nid = children_39.push_back(ChildList<39>().set_children(p)); return 0x27000000+nid;} else
    if(id ==40) {int32_t nid = children_40.push_back(ChildList<40>().set_children(p)); return 0x28000000+nid;} else {
      cout << "0error invalid id" << endl; exit(0);
    }
    return -1;
  }

  void invalidate(int32_t i) {
    int size = get_store_id(i);
    vector<SymbolPair> invalid(size,SymbolPair(-1,-1));
    set_children(i,invalid);
  }

  int get_store_id(int32_t i) {
    return (0xFF000000 & i) >> 24;
  }
  
  map<int32_t,int32_t> compact() {

    map<int32_t,int32_t> mapping;

    // remove all elements which contain -1 and create a mapping between old indexes and new ones.

    compact_vec(children_2 ,mapping,0x02000000);
    compact_vec(children_3 ,mapping,0x03000000);
    compact_vec(children_4 ,mapping,0x04000000);
    compact_vec(children_5 ,mapping,0x05000000);
    compact_vec(children_6 ,mapping,0x06000000);
    compact_vec(children_7 ,mapping,0x07000000);
    compact_vec(children_8 ,mapping,0x08000000);
    compact_vec(children_9 ,mapping,0x09000000);
    compact_vec(children_10,mapping,0x0A000000);
    compact_vec(children_11,mapping,0x0B000000);
    compact_vec(children_12,mapping,0x0C000000);
    compact_vec(children_13,mapping,0x0D000000);
    compact_vec(children_14,mapping,0x0E000000);
    compact_vec(children_15,mapping,0x0F000000);
    compact_vec(children_16,mapping,0x10000000);
    compact_vec(children_17,mapping,0x11000000);
    compact_vec(children_18,mapping,0x12000000);
    compact_vec(children_19,mapping,0x13000000);
    compact_vec(children_20,mapping,0x14000000);
    compact_vec(children_21,mapping,0x15000000);
    compact_vec(children_22,mapping,0x16000000);
    compact_vec(children_23,mapping,0x17000000);
    compact_vec(children_24,mapping,0x18000000);
    compact_vec(children_25,mapping,0x19000000);
    compact_vec(children_26,mapping,0x1A000000);
    compact_vec(children_27,mapping,0x1B000000);
    compact_vec(children_28,mapping,0x1C000000);
    compact_vec(children_29,mapping,0x1D000000);
    compact_vec(children_30,mapping,0x1E000000);
    compact_vec(children_31,mapping,0x1F000000);
    compact_vec(children_32,mapping,0x20000000);
    compact_vec(children_33,mapping,0x21000000);
    compact_vec(children_34,mapping,0x22000000);
    compact_vec(children_35,mapping,0x23000000);
    compact_vec(children_36,mapping,0x24000000);
    compact_vec(children_37,mapping,0x25000000);
    compact_vec(children_38,mapping,0x26000000);
    compact_vec(children_39,mapping,0x27000000);
    compact_vec(children_40,mapping,0x28000000);

    return mapping;
  }

  object_store_type<ChildList<2> >  children_2;
  object_store_type<ChildList<3> >  children_3;
  object_store_type<ChildList<4> >  children_4;
  object_store_type<ChildList<5> >  children_5;
  object_store_type<ChildList<6> >  children_6;
  object_store_type<ChildList<7> >  children_7;
  object_store_type<ChildList<8> >  children_8;
  object_store_type<ChildList<9> >  children_9;
  object_store_type<ChildList<10> > children_10;
  object_store_type<ChildList<11> > children_11;
  object_store_type<ChildList<12> > children_12;
  object_store_type<ChildList<13> > children_13;
  object_store_type<ChildList<14> > children_14;
  object_store_type<ChildList<15> > children_15;
  object_store_type<ChildList<16> > children_16;
  object_store_type<ChildList<17> > children_17;
  object_store_type<ChildList<18> > children_18;
  object_store_type<ChildList<19> > children_19;
  object_store_type<ChildList<20> > children_20;
  object_store_type<ChildList<21> > children_21;
  object_store_type<ChildList<22> > children_22;
  object_store_type<ChildList<23> > children_23;
  object_store_type<ChildList<24> > children_24;
  object_store_type<ChildList<25> > children_25;
  object_store_type<ChildList<26> > children_26;
  object_store_type<ChildList<27> > children_27;
  object_store_type<ChildList<28> > children_28;
  object_store_type<ChildList<29> > children_29;
  object_store_type<ChildList<30> > children_30;
  object_store_type<ChildList<31> > children_31;
  object_store_type<ChildList<32> > children_32;
  object_store_type<ChildList<33> > children_33;
  object_store_type<ChildList<34> > children_34;
  object_store_type<ChildList<35> > children_35;
  object_store_type<ChildList<36> > children_36;
  object_store_type<ChildList<37> > children_37;
  object_store_type<ChildList<38> > children_38;
  object_store_type<ChildList<39> > children_39;
  object_store_type<ChildList<40> > children_40;
};


//template<template<class T> class object_store_type=ObjectStoreMem>
class NormalSuffixNodeContainer {

  public:

    int32_t parent;
    int32_t label_start;
    int32_t suffix_link;
    int32_t next_left_leaf;
    int32_t next_right_leaf;
    int32_t depth;

    int32_t  childlist_idx;

    static int32_t invalidation_count;

    NormalSuffixNodeContainer() {
      parent          = -1;
      label_start     = -1;
      suffix_link     = -1;
      next_left_leaf  = -1;
      next_right_leaf = -1;
      depth           = -1;
      childlist_idx   = -1;
    }

    NormalSuffixNodeContainer(SuffixNode &s,ChildListStore &c,int32_t cidx) {

      parent          = s.parent;
      label_start     = s.label_start;
      suffix_link     = s.suffix_link;
      next_left_leaf  = s.next_left_leaf;
      next_right_leaf = s.next_right_leaf;
      depth           = s.depth;
      childlist_idx = -1;

      int32_t cid = c.get_store_id(cidx);
      if(cidx != -1)
      if(s.child_count() != cid) {
        // delete children idx

//        cout << "replacing my children with other children" << endl;
        // I might need to settle for periodically garbage collecting these guys.
        c.invalidate(cidx);
        invalidation_count++;
        // add new children idx
/*
        for(size_t n=0;n<s.m_children.m_symbols.size();n++) {
          cout << s.m_children.m_symbols[n].symbol;
          cout << ",";
          cout << s.m_children.m_symbols[n].index;
          cout << endl;
        }
*/
        childlist_idx = c.push_back(s.m_children.m_symbols);
        return;
      }

      if(s.child_count() == 0) {
        childlist_idx = -1;
      } else {
        if(cidx == -1) {
//          cout << "pushing back new children" << endl;
          
          cidx = c.push_back(s.m_children.m_symbols);
        } else {
//          cout << "setting children" << endl;
          c.set_children(cidx,s.m_children.m_symbols);
        }
        childlist_idx = cidx;
      }
    }

    int32_t get_depth() {
      return depth;
    }

    NormalSuffixNodeContainer(SuffixNode &s,ChildListStore &c) {

      parent          = s.parent;
      label_start     = s.label_start;
      suffix_link     = s.suffix_link;
      next_left_leaf  = s.next_left_leaf;
      next_right_leaf = s.next_right_leaf;
      depth           = s.depth;

      if(s.child_count() == 0) {
        childlist_idx = -1;
      } else {
        int32_t idx = c.push_back(s.m_children.m_symbols);
        childlist_idx = idx;
      }
    }

    int32_t child_count(ChildListStore &c) {
      return c.get_store_id(childlist_idx);
    }

    int32_t leaf_child_count(ChildListStore &c) {

      if(childlist_idx == -1) return 50;
      vector<SymbolPair> s = c.get_children(childlist_idx);

      int32_t end_children = 0;
      for(size_t n=0;n<s.size();n++) {
        if((s[n].index & 0xFF000000) > 0) end_children++;
//        else {end_children = 51; break;}
      }

      return end_children;
    }

    SuffixNode get_suffixnode(ChildListStore &c,object_store_type<NormalSuffixNodeContainer> &store) {
      SuffixNode s(0,0,0);
      s.parent = parent;
      s.label_start = label_start;

      int32_t pred_end = label_start + (depth - store.get(parent).depth)-1;
      s.label_end   = pred_end; 
      s.next_left_leaf = next_left_leaf;
      s.next_right_leaf = next_right_leaf;
      s.depth = depth;
      if(childlist_idx != -1) {
//        cout << "fetching childlist id: " << childlist_idx << endl;
        s.m_children.m_symbols = c.get_children(childlist_idx);
      } else {
//        cout << "-1 child list in normal container" << endl;
      }

// this is the last non-valid
//cout << "get suffix node" << endl;
 /*       for(size_t n=0;n<s.m_children.m_symbols.size();n++) {
          cout << (int) s.m_children.m_symbols[n].symbol;
          cout << ",";
          cout << s.m_children.m_symbols[n].index;
          cout << endl;
        }
*/
      return s;
    }

} __attribute__((__packed__));

//template<template<class T> class object_store_type=ObjectStoreMem>
class EndSuffixNodeContainer {

  public:

    int32_t parent;
    int32_t label_start;
    int32_t suffix_link;
    int32_t next_right_leaf;

    EndSuffixNodeContainer() {
  //    cout << "ENDNODE default constructor" << endl;
 //     parent = -2;
 //     label_start = -1;
 //     suffix_link = -1;
      next_right_leaf = -1;
    }

    EndSuffixNodeContainer(SuffixNode &s) {
      if(s.parent == -1) cout << "CREATING AN ENDNODE WITH PARENT OF -1 WEIRD!!!" << endl;
      parent          = s.parent;
      label_start     = s.label_start;
      suffix_link     = s.suffix_link;
      next_right_leaf = s.next_right_leaf;
    }


    SuffixNode get_suffixnode(ChildListStore &c,object_store_type<NormalSuffixNodeContainer> &store) {
      SuffixNode s(0,0,0);
      s.parent          = parent;
      s.label_start     = label_start;
      s.suffix_link     = suffix_link;
      s.next_left_leaf  = next_right_leaf;
      s.next_right_leaf = next_right_leaf;
      s.depth = store.get(s.parent).get_depth();


//      cout << "fetching parent, id of which is: " << s.parent << endl;
      if(s.parent == -2) {int *i=0; *i=1;}
      SuffixNode parent = store.get(s.parent).get_suffixnode(c,store);
//      SuffixNode parentsl = store.get(store.get(s.parent).suffix_link).get_suffixnode(c,store);

//      bool ucomp=true;
      bool found = false;
      if(store.get(s.parent).suffix_link == suffix_link) found=true;
/*
      for(int n=0;n<=40;n++) {
        int32_t id = parentsl.get_child(n);
        if(id == suffix_link) found = true;
      }
      if(!found) {cout << "slincomp" << endl; ucomp=false;}
      if(found) cout << "slcomp" << endl;
*/
//      if(label_start != parent.label_start) { cout << "label_start: " << label_start << " par: " << parent.label_end << endl; ucomp=false;}

/*
      found = false;
      for(int n=0;n<=40;n++) {
        if(next_right_leaf == parent.get_child(n)) found = true;
      }
      if(!found) {ucomp = false; cout << "nrincomp" << endl;}
      if(found) cout << "nrcomp" << endl;*/
  //    if(ucomp == true) cout << "ULTRACOMPRESS!" << endl;

      return s;
    }
} __attribute__((__packed__));

//template<template<class T> class object_store_type=ObjectStoreMem>
class SuffixNodeStore {

public:
  SuffixNodeStore() : m_rootnode(0,0,0) {
//    cout << "creating a SuffixNodeStore" << endl;
    NormalSuffixNodeContainer s(m_rootnode,m_childstore,-1);
    m_store1.push_back(s); // dummy node, root is stored elsewhere.
  }

  size_t push_back_norm() {

    NormalSuffixNodeContainer blank_node;

//    cout << "NormalSuffixNodeContainer size: " << sizeof(NormalSuffixNodeContainer) << endl;

//    cout << "NormalSuffixNodeContainer baseaddr: " << hex << &blank_node << endl;
    size_t idx = m_store1.push_back(blank_node);
    return idx;
  }

  size_t push_back_end() {
    return 0x01000000 + m_store2.push_back(EndSuffixNodeContainer());
    //return 0x01000000 + (m_store2.size()-1);
  }

  size_t push_back(SuffixNode &s) {

    if((s.label_end == -1) && (s.label_start == -1)) {
      m_rootnode = s;
      return 0;
    }

    if((s.label_end == -1) && (s.label_start != -1)) {
      EndSuffixNodeContainer s2(s);

      size_t val = m_store2.push_back(s2);
      //cout << "created id: " << val + 0x01000000 << "(store 2)" << endl;
      return val + 0x01000000;

    } else {
      
      size_t val = m_store1.push_back(NormalSuffixNodeContainer(s,m_childstore,-1));
      //cout << "created id: " << val << "(store 1)" << endl;
      return val;
    }
  }

  SuffixNode get(int idx) {

 //   cout << "SuffixNodeStore fetching id: " << idx << endl;

    if(idx == 0) {
      return m_rootnode;
    }

 //   cout << "get suffixnode: " << idx << endl;
    int id = get_store_id(idx);
    if(id == 0) { SuffixNode s = m_store1.get(idx             ).get_suffixnode(m_childstore,m_store1);
/*
cout << "get suffix node" << endl;
        for(size_t n=0;n<s.m_children.m_symbols.size();n++) {
          cout << (int) s.m_children.m_symbols[n].symbol;
          cout << ",";
          cout << s.m_children.m_symbols[n].index;
          cout << endl;
        }
*/
return s;

 }
    if(id >  0)  return m_store2.get(idx & 0x00FFFFFF).get_suffixnode(m_childstore,m_store1);

    cout << "ERRRRRRRRRRRRRRRRRRRRRRRRRRRROOOOOOORRRRRRRRRRRRRRRRR" << endl;
    exit(0);
    return m_rootnode;
  }

  void set(int idx, SuffixNode &s) {
    int id = get_store_id(idx);

    //cout << "set suffixnode: " << idx << endl;
    compact();
    if(idx == 0) {
      m_rootnode = s;
      return;
    }

//cout << "seting suffix node" << endl;
/*
        for(size_t n=0;n<s.m_children.m_symbols.size();n++) {
          cout << (int) s.m_children.m_symbols[n].symbol;
          cout << ",";
          cout << s.m_children.m_symbols[n].index;
          cout << endl;
        }
*/
    if(id == 0) m_store1.set(idx             ,NormalSuffixNodeContainer(s,m_childstore,m_store1.get(idx).childlist_idx));
    if(id >  0) m_store2.set(idx & 0x00FFFFFF,EndSuffixNodeContainer(s));
  }

  int size() {
    return m_store1.size()+m_store2.size()+1;
  }

  int get_store_id(int i) {
    return (0xFF000000 & i) >> 24;
  }

  int next_idx(int i) {
    int id = get_store_id(i);

    if(id == 0) {
      if((i+1) < static_cast<int32_t>(m_store1.size())) return i+1;
      return 0x01000000;
    } else return i++;
  }

  bool last_idx() {
    return 0x01000000+m_store2.size();
  }

  void stats() {
    cout << "Normal node count: " << m_store1.size() << endl;
    cout << "End    node count: " << m_store2.size() << endl;
    cout << "Normal node size : " << sizeof(NormalSuffixNodeContainer) << endl;
    cout << "End    node size : " << sizeof(EndSuffixNodeContainer) << endl;

    int count[100];
    for(int n=0;n<100;n++) count[n] = 0;
    for(size_t n=0;n<m_store1.size();n++) {
      int d = m_store1.get(n).child_count(m_childstore);
      count[d]++;
    }

    cout << "counts" << endl;
    for(int n=0;n<100;n++) cout << n << " " << count[n] << endl;

    int leaf_count[100];
    for(int n=0;n<100;n++) leaf_count[n] = 0;
    for(size_t n=0;n<m_store1.size();n++) {
      int d = m_store1.get(n).leaf_child_count(m_childstore);
      leaf_count[d]++;
    }
    cout << "leaf only counts: " << endl;
    for(int n=0;n<100;n++) cout << n << " " << leaf_count[n] << endl;

  }


  void compact() {
    // remove invalidated items from childstore.
    // note: this should not invalidate any SuffixNode objects that have been returned.

    if(NormalSuffixNodeContainer::invalidation_count == 100000) {
      map<int32_t,int32_t> id_mapping = m_childstore.compact();
      apply_mapping(m_store1,id_mapping);

      NormalSuffixNodeContainer::invalidation_count = 0;
    }

  }

  object_store_type<NormalSuffixNodeContainer> m_store1;
  object_store_type<EndSuffixNodeContainer>    m_store2;

  SuffixNode m_rootnode;

  ChildListStore m_childstore;
};


#endif
