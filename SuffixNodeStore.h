#ifndef SUFFIXNODESTORE
#define SUFFIXNODESTORE

#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#include "SuffixNode.h"

#include <stdint.h>

template<class v_type>
bool isinvalid(v_type &v) {

  return !v.isvalid();
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

    ChildList &set_children(vector<SymbolPair> &s) {
      for(size_t n=0;n<s.size();n++) {
        symbol[n] = s[n].symbol;
        index [n] = s[n].index;
      }
      return *this;
    }

    bool isvalid() {
      for(size_t n=0;n<childcount;n++) {if (index[n] != -1) return true;}
      return false;
    }

    int32_t symbol[childcount];
    int32_t index [childcount];
};

class ChildListStore {

public:

  vector<SymbolPair> get_children(int32_t idx) {

    int id = get_store_id(idx);
    if(id == 1) {cout << "GETerror nodes with one child should not be possible" << endl; children_2[10000000] = -1; }
    if(id == 2 ) return children_2 [0x00FFFFFF & idx].get_childlist();
    if(id == 3 ) return children_3 [0x00FFFFFF & idx].get_childlist();
    if(id == 4 ) return children_4 [0x00FFFFFF & idx].get_childlist();
    if(id == 5 ) return children_5 [0x00FFFFFF & idx].get_childlist();
    if(id == 6 ) return children_6 [0x00FFFFFF & idx].get_childlist();
    if(id == 7 ) return children_7 [0x00FFFFFF & idx].get_childlist();
    if(id == 8 ) return children_8 [0x00FFFFFF & idx].get_childlist();
    if(id == 9 ) return children_9 [0x00FFFFFF & idx].get_childlist();
    if(id == 10) return children_10[0x00FFFFFF & idx].get_childlist();
    if(id == 11) return children_11[0x00FFFFFF & idx].get_childlist();
    if(id == 12) return children_12[0x00FFFFFF & idx].get_childlist();
    if(id == 13) return children_13[0x00FFFFFF & idx].get_childlist();
    if(id == 14) return children_14[0x00FFFFFF & idx].get_childlist();
    if(id == 15) return children_15[0x00FFFFFF & idx].get_childlist();
    if(id == 16) return children_16[0x00FFFFFF & idx].get_childlist();
    if(id == 17) return children_17[0x00FFFFFF & idx].get_childlist();
    if(id == 18) return children_18[0x00FFFFFF & idx].get_childlist();
    if(id == 19) return children_19[0x00FFFFFF & idx].get_childlist();
    if(id == 20) return children_20[0x00FFFFFF & idx].get_childlist();
    if(id == 21) return children_21[0x00FFFFFF & idx].get_childlist();
    if(id == 22) return children_22[0x00FFFFFF & idx].get_childlist();
    if(id == 23) return children_23[0x00FFFFFF & idx].get_childlist();
    if(id == 24) return children_24[0x00FFFFFF & idx].get_childlist();
    if(id == 25) return children_25[0x00FFFFFF & idx].get_childlist();
    if(id == 26) return children_26[0x00FFFFFF & idx].get_childlist();
    if(id == 27) return children_27[0x00FFFFFF & idx].get_childlist();
    if(id == 28) return children_28[0x00FFFFFF & idx].get_childlist();
    if(id == 29) return children_29[0x00FFFFFF & idx].get_childlist();
    if(id == 30) return children_30[0x00FFFFFF & idx].get_childlist();
    if(id == 31) return children_31[0x00FFFFFF & idx].get_childlist();
    if(id == 32) return children_32[0x00FFFFFF & idx].get_childlist();
    if(id == 33) return children_33[0x00FFFFFF & idx].get_childlist();
    if(id == 34) return children_34[0x00FFFFFF & idx].get_childlist();
    if(id == 35) return children_35[0x00FFFFFF & idx].get_childlist();
    if(id == 36) return children_36[0x00FFFFFF & idx].get_childlist();
    if(id == 37) return children_37[0x00FFFFFF & idx].get_childlist();
    if(id == 38) return children_38[0x00FFFFFF & idx].get_childlist();
    if(id == 39) return children_39[0x00FFFFFF & idx].get_childlist();
    if(id == 40) return children_40[0x00FFFFFF & idx].get_childlist();
    cout << "1error invalid id" << endl; children_2[10000000] = -1;
  }

  void set_children(int32_t idx,vector<SymbolPair> &p) {

    int32_t id = get_store_id(idx);
    if(id != p.size()) { cout << "ERROR: new size does not equal old size cannot use same idx" << endl; return; }

    if(id == 1) {cout << "error nodes with one child should not be possible" << endl; children_2[10000000] = -1; } else
    if(id == 2) { for(size_t i=0;i<p.size();i++) { children_2 [idx-0x02000000].symbol[i] = p[i].symbol; children_2 [idx-0x02000000].index[i] = p[i].index; } } else
    if(id == 3) { for(size_t i=0;i<p.size();i++) { children_3 [idx-0x03000000].symbol[i] = p[i].symbol; children_3 [idx-0x03000000].index[i] = p[i].index; } } else
    if(id == 4) { for(size_t i=0;i<p.size();i++) { children_4 [idx-0x04000000].symbol[i] = p[i].symbol; children_4 [idx-0x04000000].index[i] = p[i].index; } } else
    if(id == 5) { for(size_t i=0;i<p.size();i++) { children_5 [idx-0x05000000].symbol[i] = p[i].symbol; children_5 [idx-0x05000000].index[i] = p[i].index; } } else
    if(id == 6) { for(size_t i=0;i<p.size();i++) { children_6 [idx-0x06000000].symbol[i] = p[i].symbol; children_6 [idx-0x06000000].index[i] = p[i].index; } } else
    if(id == 7) { for(size_t i=0;i<p.size();i++) { children_7 [idx-0x07000000].symbol[i] = p[i].symbol; children_7 [idx-0x07000000].index[i] = p[i].index; } } else
    if(id == 8) { for(size_t i=0;i<p.size();i++) { children_8 [idx-0x08000000].symbol[i] = p[i].symbol; children_8 [idx-0x08000000].index[i] = p[i].index; } } else
    if(id == 9) { for(size_t i=0;i<p.size();i++) { children_9 [idx-0x09000000].symbol[i] = p[i].symbol; children_9 [idx-0x09000000].index[i] = p[i].index; } } else
    if(id ==10) { for(size_t i=0;i<p.size();i++) { children_10[idx-0x0A000000].symbol[i] = p[i].symbol; children_10[idx-0x0A000000].index[i] = p[i].index; } } else
    if(id ==11) { for(size_t i=0;i<p.size();i++) { children_11[idx-0x0B000000].symbol[i] = p[i].symbol; children_11[idx-0x0B000000].index[i] = p[i].index; } } else
    if(id ==12) { for(size_t i=0;i<p.size();i++) { children_12[idx-0x0C000000].symbol[i] = p[i].symbol; children_12[idx-0x0C000000].index[i] = p[i].index; } } else
    if(id ==13) { for(size_t i=0;i<p.size();i++) { children_13[idx-0x0D000000].symbol[i] = p[i].symbol; children_13[idx-0x0D000000].index[i] = p[i].index; } } else
    if(id ==14) { for(size_t i=0;i<p.size();i++) { children_14[idx-0x0E000000].symbol[i] = p[i].symbol; children_14[idx-0x0E000000].index[i] = p[i].index; } } else
    if(id ==15) { for(size_t i=0;i<p.size();i++) { children_15[idx-0x0F000000].symbol[i] = p[i].symbol; children_15[idx-0x0F000000].index[i] = p[i].index; } } else
    if(id ==16) { for(size_t i=0;i<p.size();i++) { children_16[idx-0x10000000].symbol[i] = p[i].symbol; children_16[idx-0x10000000].index[i] = p[i].index; } } else
    if(id ==17) { for(size_t i=0;i<p.size();i++) { children_17[idx-0x11000000].symbol[i] = p[i].symbol; children_17[idx-0x11000000].index[i] = p[i].index; } } else
    if(id ==18) { for(size_t i=0;i<p.size();i++) { children_18[idx-0x12000000].symbol[i] = p[i].symbol; children_18[idx-0x12000000].index[i] = p[i].index; } } else
    if(id ==19) { for(size_t i=0;i<p.size();i++) { children_19[idx-0x13000000].symbol[i] = p[i].symbol; children_19[idx-0x13000000].index[i] = p[i].index; } } else
    if(id ==20) { for(size_t i=0;i<p.size();i++) { children_20[idx-0x14000000].symbol[i] = p[i].symbol; children_20[idx-0x14000000].index[i] = p[i].index; } } else
    if(id ==21) { for(size_t i=0;i<p.size();i++) { children_21[idx-0x15000000].symbol[i] = p[i].symbol; children_21[idx-0x15000000].index[i] = p[i].index; } } else
    if(id ==22) { for(size_t i=0;i<p.size();i++) { children_22[idx-0x16000000].symbol[i] = p[i].symbol; children_22[idx-0x16000000].index[i] = p[i].index; } } else
    if(id ==23) { for(size_t i=0;i<p.size();i++) { children_23[idx-0x17000000].symbol[i] = p[i].symbol; children_23[idx-0x17000000].index[i] = p[i].index; } } else
    if(id ==24) { for(size_t i=0;i<p.size();i++) { children_24[idx-0x18000000].symbol[i] = p[i].symbol; children_24[idx-0x18000000].index[i] = p[i].index; } } else
    if(id ==25) { for(size_t i=0;i<p.size();i++) { children_25[idx-0x19000000].symbol[i] = p[i].symbol; children_25[idx-0x19000000].index[i] = p[i].index; } } else
    if(id ==26) { for(size_t i=0;i<p.size();i++) { children_26[idx-0x1A000000].symbol[i] = p[i].symbol; children_26[idx-0x1A000000].index[i] = p[i].index; } } else
    if(id ==27) { for(size_t i=0;i<p.size();i++) { children_27[idx-0x1B000000].symbol[i] = p[i].symbol; children_27[idx-0x1B000000].index[i] = p[i].index; } } else
    if(id ==28) { for(size_t i=0;i<p.size();i++) { children_28[idx-0x1C000000].symbol[i] = p[i].symbol; children_28[idx-0x1C000000].index[i] = p[i].index; } } else
    if(id ==29) { for(size_t i=0;i<p.size();i++) { children_29[idx-0x1D000000].symbol[i] = p[i].symbol; children_29[idx-0x1D000000].index[i] = p[i].index; } } else
    if(id ==30) { for(size_t i=0;i<p.size();i++) { children_30[idx-0x1E000000].symbol[i] = p[i].symbol; children_30[idx-0x1E000000].index[i] = p[i].index; } } else
    if(id ==31) { for(size_t i=0;i<p.size();i++) { children_31[idx-0x1F000000].symbol[i] = p[i].symbol; children_31[idx-0x1F000000].index[i] = p[i].index; } } else
    if(id ==32) { for(size_t i=0;i<p.size();i++) { children_32[idx-0x20000000].symbol[i] = p[i].symbol; children_32[idx-0x20000000].index[i] = p[i].index; } } else
    if(id ==33) { for(size_t i=0;i<p.size();i++) { children_33[idx-0x21000000].symbol[i] = p[i].symbol; children_33[idx-0x21000000].index[i] = p[i].index; } } else
    if(id ==34) { for(size_t i=0;i<p.size();i++) { children_34[idx-0x22000000].symbol[i] = p[i].symbol; children_34[idx-0x22000000].index[i] = p[i].index; } } else
    if(id ==35) { for(size_t i=0;i<p.size();i++) { children_35[idx-0x23000000].symbol[i] = p[i].symbol; children_35[idx-0x23000000].index[i] = p[i].index; } } else
    if(id ==36) { for(size_t i=0;i<p.size();i++) { children_36[idx-0x24000000].symbol[i] = p[i].symbol; children_36[idx-0x24000000].index[i] = p[i].index; } } else
    if(id ==37) { for(size_t i=0;i<p.size();i++) { children_37[idx-0x25000000].symbol[i] = p[i].symbol; children_37[idx-0x25000000].index[i] = p[i].index; } } else
    if(id ==38) { for(size_t i=0;i<p.size();i++) { children_38[idx-0x26000000].symbol[i] = p[i].symbol; children_38[idx-0x26000000].index[i] = p[i].index; } } else
    if(id ==39) { for(size_t i=0;i<p.size();i++) { children_39[idx-0x27000000].symbol[i] = p[i].symbol; children_38[idx-0x27000000].index[i] = p[i].index; } } else
    if(id ==40) { for(size_t i=0;i<p.size();i++) { children_40[idx-0x28000000].symbol[i] = p[i].symbol; children_38[idx-0x28000000].index[i] = p[i].index; } } else
    { cout << "2error invalid id" << endl; children_2[10000000] = -1; }
  }

  int32_t push_back(vector<SymbolPair> &p) {

    // 1. find our which storage section to put it in.
    int id = p.size();
    if(id == 1) {cout << "PUSHerror nodes with one child should not be possible" << endl; children_2[10000000] = -1; }
    // 2. put it there
    // 3. return the size()-1
    if(id == 2) {children_2 .push_back(ChildList<2 >().set_children(p)); return 0x02000000+(children_2 .size()-1);}
    if(id == 3) {children_3 .push_back(ChildList<3 >().set_children(p)); return 0x03000000+(children_3 .size()-1);}
    if(id == 4) {children_4 .push_back(ChildList<4 >().set_children(p)); return 0x04000000+(children_4 .size()-1);}
    if(id == 5) {children_5 .push_back(ChildList<5 >().set_children(p)); return 0x05000000+(children_5 .size()-1);}
    if(id == 6) {children_6 .push_back(ChildList<6 >().set_children(p)); return 0x06000000+(children_6 .size()-1);}
    if(id == 7) {children_7 .push_back(ChildList<7 >().set_children(p)); return 0x07000000+(children_7 .size()-1);}
    if(id == 8) {children_8 .push_back(ChildList<8 >().set_children(p)); return 0x08000000+(children_8 .size()-1);}
    if(id == 9) {children_9 .push_back(ChildList<9 >().set_children(p)); return 0x09000000+(children_9 .size()-1);}
    if(id ==10) {children_10.push_back(ChildList<10>().set_children(p)); return 0x0A000000+(children_10.size()-1);}
    if(id ==11) {children_11.push_back(ChildList<11>().set_children(p)); return 0x0B000000+(children_11.size()-1);}
    if(id ==12) {children_12.push_back(ChildList<12>().set_children(p)); return 0x0C000000+(children_12.size()-1);}
    if(id ==13) {children_13.push_back(ChildList<13>().set_children(p)); return 0x0D000000+(children_13.size()-1);}
    if(id ==14) {children_14.push_back(ChildList<14>().set_children(p)); return 0x0E000000+(children_14.size()-1);}
    if(id ==15) {children_15.push_back(ChildList<15>().set_children(p)); return 0x0F000000+(children_15.size()-1);}
    if(id ==16) {children_16.push_back(ChildList<16>().set_children(p)); return 0x10000000+(children_16.size()-1);}
    if(id ==17) {children_17.push_back(ChildList<17>().set_children(p)); return 0x11000000+(children_17.size()-1);}
    if(id ==18) {children_18.push_back(ChildList<18>().set_children(p)); return 0x12000000+(children_18.size()-1);}
    if(id ==19) {children_19.push_back(ChildList<19>().set_children(p)); return 0x13000000+(children_19.size()-1);}
    if(id ==20) {children_20.push_back(ChildList<20>().set_children(p)); return 0x14000000+(children_20.size()-1);}
    if(id ==21) {children_21.push_back(ChildList<21>().set_children(p)); return 0x15000000+(children_21.size()-1);}
    if(id ==22) {children_22.push_back(ChildList<22>().set_children(p)); return 0x16000000+(children_22.size()-1);}
    if(id ==23) {children_23.push_back(ChildList<23>().set_children(p)); return 0x17000000+(children_23.size()-1);}
    if(id ==24) {children_24.push_back(ChildList<24>().set_children(p)); return 0x18000000+(children_24.size()-1);}
    if(id ==25) {children_25.push_back(ChildList<25>().set_children(p)); return 0x19000000+(children_25.size()-1);}
    if(id ==26) {children_26.push_back(ChildList<26>().set_children(p)); return 0x1A000000+(children_26.size()-1);}
    if(id ==27) {children_27.push_back(ChildList<27>().set_children(p)); return 0x1B000000+(children_27.size()-1);}
    if(id ==28) {children_28.push_back(ChildList<28>().set_children(p)); return 0x1C000000+(children_28.size()-1);}
    if(id ==29) {children_29.push_back(ChildList<29>().set_children(p)); return 0x1D000000+(children_29.size()-1);}
    if(id ==30) {children_30.push_back(ChildList<30>().set_children(p)); return 0x1E000000+(children_30.size()-1);}
    if(id ==31) {children_31.push_back(ChildList<31>().set_children(p)); return 0x1F000000+(children_31.size()-1);}
    if(id ==32) {children_32.push_back(ChildList<32>().set_children(p)); return 0x20000000+(children_32.size()-1);}
    if(id ==33) {children_33.push_back(ChildList<33>().set_children(p)); return 0x21000000+(children_33.size()-1);}
    if(id ==34) {children_34.push_back(ChildList<34>().set_children(p)); return 0x22000000+(children_34.size()-1);}
    if(id ==35) {children_35.push_back(ChildList<35>().set_children(p)); return 0x23000000+(children_35.size()-1);}
    if(id ==36) {children_36.push_back(ChildList<36>().set_children(p)); return 0x24000000+(children_36.size()-1);}
    if(id ==37) {children_37.push_back(ChildList<37>().set_children(p)); return 0x25000000+(children_37.size()-1);}
    if(id ==38) {children_38.push_back(ChildList<38>().set_children(p)); return 0x26000000+(children_38.size()-1);}
    if(id ==39) {children_39.push_back(ChildList<39>().set_children(p)); return 0x27000000+(children_39.size()-1);}
    if(id ==40) {children_40.push_back(ChildList<40>().set_children(p)); return 0x28000000+(children_40.size()-1);}
    cout << "0error invalid id" << endl; children_2[10000000] = -1;
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


  template<class vec_type>
  void compact_vec(vec_type &v,map<int32_t,int32_t> &mapping,int32_t id) {

    // Generate mapping information
    size_t new_idx = 0;
    for(size_t n=0;n<v.size();n++) {
      if(v[n].isvalid()) {
        //cout << "mapping: " << n+id << " to " << new_idx+id << endl;
        mapping[n+id] = new_idx + id;
        new_idx++;
      }
    }

    // Clean up the vector
    size_t old_size = v.size();
    cout << "pre compact child store size: " << v.size();
    v.erase(remove_if(v.begin(), v.end(), isinvalid<typename vec_type::value_type> ), v.end());
    cout << "post: " << v.size() << " removed: " << old_size-v.size() << endl;
  }

  vector<ChildList<2> >  children_2;
  vector<ChildList<3> >  children_3;
  vector<ChildList<4> >  children_4;
  vector<ChildList<5> >  children_5;
  vector<ChildList<6> >  children_6;
  vector<ChildList<7> >  children_7;
  vector<ChildList<8> >  children_8;
  vector<ChildList<9> >  children_9;
  vector<ChildList<10> > children_10;
  vector<ChildList<11> > children_11;
  vector<ChildList<12> > children_12;
  vector<ChildList<13> > children_13;
  vector<ChildList<14> > children_14;
  vector<ChildList<15> > children_15;
  vector<ChildList<16> > children_16;
  vector<ChildList<17> > children_17;
  vector<ChildList<18> > children_18;
  vector<ChildList<19> > children_19;
  vector<ChildList<20> > children_20;
  vector<ChildList<21> > children_21;
  vector<ChildList<22> > children_22;
  vector<ChildList<23> > children_23;
  vector<ChildList<24> > children_24;
  vector<ChildList<25> > children_25;
  vector<ChildList<26> > children_26;
  vector<ChildList<27> > children_27;
  vector<ChildList<28> > children_28;
  vector<ChildList<29> > children_29;
  vector<ChildList<30> > children_30;
  vector<ChildList<31> > children_31;
  vector<ChildList<32> > children_32;
  vector<ChildList<33> > children_33;
  vector<ChildList<34> > children_34;
  vector<ChildList<35> > children_35;
  vector<ChildList<36> > children_36;
  vector<ChildList<37> > children_37;
  vector<ChildList<38> > children_38;
  vector<ChildList<39> > children_39;
  vector<ChildList<40> > children_40;
};

class NormalSuffixNodeContainer {

  public:

    int32_t parent;
    int32_t label_start;
    int32_t label_end;
    int32_t suffix_link;
    int32_t next_left_leaf;
    int32_t next_right_leaf;
    int32_t depth;

    int32_t childlist_idx;

    static int32_t invalidation_count;

    NormalSuffixNodeContainer(SuffixNode &s,ChildListStore &c,int32_t cidx) {

      parent          = s.parent;
      label_start     = s.label_start;
      label_end       = s.label_end;
      suffix_link     = s.suffix_link;
      next_left_leaf  = s.next_left_leaf;
      next_right_leaf = s.next_right_leaf;
      depth           = s.depth;
      childlist_idx = -1;

      int32_t cid = c.get_store_id(cidx);
      if(cidx != -1)
      if(s.child_count() != cid) {
        // delete children idx

        // I might need to settle for periodically garbage collecting these guys.
        c.invalidate(cidx);
        invalidation_count++;
        // add new children idx
        childlist_idx = c.push_back(s.m_children.m_symbols);
        return;
      }

      if(s.child_count() == 0) {
       // int *i = 0; *i=1;
        childlist_idx = -1;
      } else {
        if(cidx == -1) cidx = c.push_back(s.m_children.m_symbols);
        c.set_children(cidx,s.m_children.m_symbols);
        childlist_idx = cidx;
      }
    }

    NormalSuffixNodeContainer(SuffixNode &s,ChildListStore &c) {

      parent          = s.parent;
      label_start     = s.label_start;
      label_end       = s.label_end;
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

    SuffixNode get_suffixnode(ChildListStore &c) {
      SuffixNode s(0,0,0);
      s.parent = parent;
      s.label_start = label_start;
      s.label_end   = label_end;
      s.next_left_leaf = next_left_leaf;
      s.next_right_leaf = next_right_leaf;
      s.depth = depth;
      if(childlist_idx != -1) s.m_children.m_symbols = c.get_children(childlist_idx);
      return s;
    }

};

class EndSuffixNodeContainer {

  public:

    int32_t parent;
    int32_t label_start;
    int32_t suffix_link;
    int32_t next_right_leaf;
    int32_t depth;

    EndSuffixNodeContainer() {
    }

    EndSuffixNodeContainer(SuffixNode &s) {
      parent          = s.parent;
      label_start     = s.label_start;
      suffix_link     = s.suffix_link;
      next_right_leaf = s.next_right_leaf;
      depth           = s.depth;
    }


    SuffixNode get_suffixnode() {
      SuffixNode s(0,0,0);
      s.parent          = parent;
      s.label_start     = label_start;
      s.suffix_link     = suffix_link;
      s.next_left_leaf  = next_right_leaf;
      s.next_right_leaf = next_right_leaf;
      s.depth           = depth;

      return s;
    }
};

class SuffixNodeStore {

public:
  SuffixNodeStore() : m_rootnode(0,0,0) {
    NormalSuffixNodeContainer s(m_rootnode,m_childstore,-1);
    m_store1.push_back(s); // dummy node, root is stored elsewhere.
  }

  size_t push_back_norm() {
    SuffixNode s(0,0,0);
    m_store1.push_back(NormalSuffixNodeContainer(s,m_childstore,-1));
    return m_store1.size()-1;
  }

  size_t push_back_end() {
    m_store2.push_back(EndSuffixNodeContainer());
    return 0x01000000 + (m_store2.size()-1);
  }

  size_t push_back(SuffixNode &s) {

    if((s.label_end == -1) && (s.label_start == -1)) {
      m_rootnode = s;
      return 0;
    }

    if((s.label_end == -1) && (s.label_start != -1)) {
      EndSuffixNodeContainer s2(s);

      m_store2.push_back(s2);
      return m_store2.size()-1 + 0x01000000;

    } else {
      m_store1.push_back(NormalSuffixNodeContainer(s,m_childstore,-1));
      return m_store1.size()-1;
    }
  }

  SuffixNode get(int idx) {

    if(idx == 0) {
      return m_rootnode;
    }

    int id = get_store_id(idx);
    if(id == 0) return m_store1[idx           ].get_suffixnode(m_childstore);
    if(id == 1) return m_store2[idx-0x01000000].get_suffixnode();
  }

  void set(int idx, SuffixNode &s) {
    int id = get_store_id(idx);

    compact();
    if(idx == 0) {
      m_rootnode = s;
      return;
    }


    if(id == 0) m_store1[idx           ] = NormalSuffixNodeContainer(s,m_childstore,m_store1[idx].childlist_idx);
    if(id == 1) m_store2[idx-0x01000000] = EndSuffixNodeContainer(s);
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
      if(i+1 < m_store1.size()) return i+1;
      return 0x01000000;
    } else return i++;
  }

  bool last_idx() {
    return 0x01000000+m_store2.size();
  }


  void stats() {
    cout << "Normal node count: " << m_store1.size() << endl;
    cout << "End    node count: " << m_store2.size() << endl;

    int count[100];
    for(int n=0;n<100;n++) count[n] = 0;
    for(size_t n=0;n<m_store1.size();n++) {
      int d = m_store1[n].child_count(m_childstore);
      count[d]++;
    }

    cout << "counts" << endl;
    for(int n=0;n<100;n++) cout << n << " " << count[n] << endl;
  }

  void apply_mapping(map<int32_t,int32_t> &mapping) {
 //   cout << "mapping size: " << mapping.size() << endl;

    for(size_t n=0;n<m_store1.size();n++) {
      if(m_store1[n].childlist_idx != -1) m_store1[n].childlist_idx = mapping[m_store1[n].childlist_idx];
    }
  }

  void compact() {
    // remove invalidated items from childstore.
    // note: this should not invalidate any SuffixNode objects that have been returned.

    if(NormalSuffixNodeContainer::invalidation_count == 100000) {
      map<int32_t,int32_t> id_mapping = m_childstore.compact();
      apply_mapping(id_mapping);

      NormalSuffixNodeContainer::invalidation_count = 0;
    }
  }

  vector<NormalSuffixNodeContainer> m_store1;
  vector<EndSuffixNodeContainer   > m_store2;

  SuffixNode m_rootnode;

  ChildListStore m_childstore;
};

int32_t NormalSuffixNodeContainer::invalidation_count = 0;

#endif
