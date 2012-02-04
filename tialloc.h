#ifndef tialloc_h
#define tialloc_h

#include <stdint.h>

#if __x86_64__
#define ENV64BIT
typedef uint64_t intarch_t;
#else
#define ENV32BIT
typedef uint32_t intarch_t;
#endif

#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>

using namespace std;

struct small_block {
  //small_block *next_free_block;
  small_block *prev_free_block;
  uint8_t         free_chain_start;   // 0xFF means unallocated, otherwise index in to data.
  uint8_t         free_chain_end;     // 0xFF means unallocated, otherwise index in to data.
  uint8_t         alloc_size;

  #ifdef ENV32BIT
  uint8_t         data[249];
  #endif
  #ifdef ENV64BIT
  uint8_t         data[245];
  #endif
} __attribute__((__packed__));

class tialloc {

  small_block *free_block_ptr[256]; // pointers to last_free_blocks
  vector<vector<uint8_t *> > all_memory;    // this isn't required, and should be removed from non-debug implementations.
  int max_tiallocation; ///< must be less than 255, ideally around 120.
  
  static tialloc *only_instance;

private:
  tialloc() {
    max_tiallocation = 121;
    all_memory = vector<vector<uint8_t *> >(max_tiallocation+1,vector<uint8_t *>());
    for(int n=1;n<=max_tiallocation;n++) {
      initialise(n);
    }
  }

public:
  static tialloc *instance() {
    if(only_instance == NULL) only_instance = new tialloc();
    return only_instance;
  }

  // Return true if this block was allocated by tialloc, otherwise false (malloc'd)
  bool is_tiallocated(void *addr) {

    //if((((intarch_t)addr) & 0x7) > 0) return true;
    //if(((intarch_t)addr)%8 != 0) return true;
    //int s = malloc_usable_size(addr);
    //if((s < 0) || (s > max_tiallocation) || (s == 0)) return false;

    small_block *this_block = get_tiblock_start(addr);
    //cout << "aaaddr    : " << (uint64_t) addr << endl;
    //cout << "this_block: " << (uint64_t) this_block << endl;
    if(this_block->alloc_size > max_tiallocation) return false;
                                             else return true;
  }

  intarch_t alloc_size(void *addr) {
    if(is_tiallocated(addr)) {
      small_block *a = get_tiblock_start(addr);
      return a->alloc_size;
    } else {
      #ifdef __APPLE__
      return malloc_size(get_tiblock_start(addr))-24; // ask malloc for it's size.
      #else
      return malloc_usable_size(get_tiblock_start(addr))-24;
      #endif
    }
  }


  // get the start of the tialloc block from any pointer within it.
  small_block *get_tiblock_start(void *addr) {
    return (small_block *)((intarch_t) addr & 0xFFFFFFFFFFFFFF00);
  }

  // from a pointer to a data item in a tialloc block, return the data item index
  uint8_t ptr_to_data_idx(void *addr) {

    small_block *block_addr = get_tiblock_start(addr);

    return ((intarch_t)addr)-(intarch_t)(block_addr->data);
  }

  // from a pointer to a data item in a tialloc block, return the data item index
  uint8_t ptr_to_data_idx(small_block *block_addr,void *addr) {

    return ((intarch_t)addr)-(intarch_t)(block_addr->data);
  }

  bool any_free(small_block *addr) {

    if((uint64_t) addr == 0) return false;

    if((addr->free_chain_start == 0xFF) && (addr->free_chain_end == 0xFF)) return false;
    return true;
  }

  void free(void *addr) {

    if(!is_tiallocated(addr)) {
      //cout << "free: " << (uint64_t) ((uint8_t *) addr)-24 << endl;
      ::free(((uint8_t *) addr)-24);
      return;
    }

    small_block *this_block = get_tiblock_start(addr);

    // are there existing free elements in this block?
    if(any_free(this_block)) {
      // there are existing free items
//cout << "exist free add" << endl;
      uint8_t d_temp = this_block->free_chain_end;
      
      uint8_t free_item_idx = ptr_to_data_idx(this_block,addr);
      this_block->data[free_item_idx] = d_temp;

      this_block->free_chain_end = free_item_idx;
      return; 
    } else {
      // there are no existing free items
//cout << "no cur free add" << endl;
      small_block *fb_temp = free_block_ptr[this_block->alloc_size];

      free_block_ptr[this_block->alloc_size] = this_block;

      this_block->free_chain_start = ptr_to_data_idx(this_block,addr);
      this_block->free_chain_end   = this_block->free_chain_start;

      if(fb_temp != 0) this_block->prev_free_block = fb_temp;
      return;
    }
  }

  void *realloc(void *addr, intarch_t new_size) {

     int osize = alloc_size(addr);
     if(is_tiallocated(addr) && (new_size == osize)) return addr;

     void *newaddr = alloc(new_size);

     size_t size;
     if(osize < new_size) size = osize;
                     else size = new_size;

     for(size_t n=0;n<size;n++) {
       *(((char *) newaddr)+n) = *(((char *) addr) + n);
     }
     free(addr);
     return newaddr;
  }

  void *alloc(intarch_t n_alloc_size=1) {

    //cout << "enteralloc" << endl;
    //dump(n_alloc_size);

    if(n_alloc_size == 0) return 0;

    if(n_alloc_size > max_tiallocation) {
      void *a;
      posix_memalign(&a,512,n_alloc_size+24);
      //cout << "alloc: " << (uint64_t) (uint8_t *)a << endl;

      for(size_t n=0;n<24;n++) { ((uint8_t *)a)[n] = 0xFF; }
      return ((uint8_t *)a+24);
    }

    // are there any free items left?
    if((free_block_ptr[n_alloc_size] != 0) && any_free(free_block_ptr[n_alloc_size])) {

      // only one item left?
      if(free_block_ptr[n_alloc_size]->free_chain_start != free_block_ptr[n_alloc_size]->free_chain_end) {
        // more than one item left.
        //cout << "more than one free alloc" << n_alloc_size << endl;
        uint8_t ret_idx = free_block_ptr[n_alloc_size]->free_chain_end;

        free_block_ptr[n_alloc_size]->free_chain_end = free_block_ptr[n_alloc_size]->data[free_block_ptr[n_alloc_size]->free_chain_end];

        //dump(91);
        void *retval = (void *) (((intarch_t)free_block_ptr[n_alloc_size]->data) + ret_idx);
        return retval;
      } else { 
        //cout << "only one free alloc " << n_alloc_size << endl;
        uint8_t ret_idx = free_block_ptr[n_alloc_size]->free_chain_end;
        free_block_ptr[n_alloc_size]->free_chain_start = 0xFF;
        free_block_ptr[n_alloc_size]->free_chain_end   = 0xFF;
        void *retval = (void *) (((char *) free_block_ptr[n_alloc_size]->data) + ret_idx);

        small_block *free_block_tmp = free_block_ptr[n_alloc_size]->prev_free_block;
        free_block_ptr[n_alloc_size]->prev_free_block  = 0;

        free_block_ptr[n_alloc_size] = free_block_tmp;
        //dump(n_alloc_size);

        //cout << "free blk ptr now: " << (uint64_t) free_block_ptr[n_alloc_size] << endl;

        return retval;
      }


    } else {
      //cout << "no more free init" << endl;

      //cout << "alloc size: " << n_alloc_size << endl;
      //cout << "free_block_ptr:" << (uint64_t) free_block_ptr[n_alloc_size] << endl;

      // allocate more blocks here
      initialise(n_alloc_size);
      return alloc(n_alloc_size);
    }

  }

  void initialise(int n_alloc_size = 1) {

    //cout << "Size of smallblock: " << sizeof(small_block);

    intarch_t m_alloc_size = 1024*1000; 

    // malloc a bunch of memory
    // memory should really only be local, eventually.
    //cout << "alloc size: " << n_alloc_size << endl;
    void *m;// = (uint8_t *)memalign (256,m_alloc_size); 
    posix_memalign(&m,512,m_alloc_size);
    uint8_t *memory = (uint8_t *) m;

    //cout << "memory: " << (intarch_t) memory << endl;
    
    if(memory == NULL) {cerr << "allocation error" << endl;}

    all_memory[n_alloc_size].push_back(memory);

    for(;((intarch_t)memory)%256 !=0;) {cout << "SHOULD NEVER DO THIS" << endl; memory++; m_alloc_size--;} // pad allocation to be a multiple of 256

    #ifdef ENV32BIT
    int data_size = 249;
    #endif
    #ifdef ENV64BIT
    int data_size = 245;
    #endif

    small_block t;
    t.prev_free_block  = 0;
    //t.next_free_block  = 0;
    t.alloc_size       = n_alloc_size;
    t.free_chain_start = 0;
    //cout << "alloc size: " << n_alloc_size << " fce: " << 244 - (244%n_alloc_size) - n_alloc_size << endl;
    t.free_chain_end   = (data_size-1) - ((data_size-1)%n_alloc_size) - n_alloc_size; // added - n_alloc_size
    for(int n=n_alloc_size;n<data_size;n=n+n_alloc_size) {
      t.data[n] = n-n_alloc_size;
    }
    t.data[0] = 0xFF;

    void *current_addr = memory;
    int n_limit = ((m_alloc_size/256));
    //cout << "n_limit: " << n_limit << endl;
    for(int n=0;n<n_limit;n++) {
      t.prev_free_block = (small_block *)(((intarch_t)current_addr)-256);
      if(n==0) t.prev_free_block = 0;

      memcpy(current_addr,&t,256);

      //cout << "current_addr: " << (uint64_t) current_addr << endl;
      current_addr = (void *) ((intarch_t) current_addr + 256);
    }

    //cout << "lastblk " << n_alloc_size << " : " <<  ((intarch_t) current_addr - 256) << endl;

    free_block_ptr[n_alloc_size] = (small_block *) ((intarch_t) current_addr - 256);
    //dump(n_alloc_size);
  }

  void dump(size_t alloc_size) {

    cout << "dump " << alloc_size << endl;
    small_block *c = free_block_ptr[alloc_size];
    for(;(intarch_t) c != 0;) {
      cout << "block: " << (uint64_t) c << endl;
      c = c->prev_free_block;
    }

  }

};

#endif
