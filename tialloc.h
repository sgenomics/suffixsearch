#ifndef tialloc_h
#define tialloc_h

#include <stdint.h>
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include <iostream>
#include <vector>
#include <string.h>


using namespace std;

struct small_block {
  small_block *next_free_block;
  small_block *prev_free_block;
   uint8_t         free_chain_start;   // 0xFF means unallocated, otherwise index in to data.
   uint8_t         free_chain_end;     // 0xFF means unallocated, otherwise index in to data.
   uint8_t         alloc_size;

   uint8_t         data[245];
} __attribute__((__packed__));

class tialloc {

  small_block *free_block_ptr[123]; // pointers to last_free_blocks
  int32_t m_alloc_size;
  vector<uint8_t *> all_memory;    // this isn't required, and should be removed from non-debug implementations.
  int max_tiallocation;

  
  static tialloc *only_instance;

private:
  tialloc() {
    max_tiallocation = 122;
    for(int n=1;n<123;n++) {
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
    if(((int32_t)addr)%4 == 0) {  // all malloc'd blocks will be word aligned.
      void *aligned_addr = (void *)(((int32_t)addr) - (int32_t)addr%4);

      #ifdef __APPLE__
      long int size = malloc_size(aligned_addr); // ask malloc for it's size.
      #else
      long int size = malloc_usable_size(aligned_addr);
      #endif

      if(size != 0) {
        // This block was malloc'd
        return false;
      }
    }

    return true;
  }

  int32_t alloc_size(void *addr) {
    if(is_tiallocated(addr)) {
      small_block *a = get_tiblock_start(addr);
      return a->alloc_size;
    } else {
      #ifdef __APPLE__
      return malloc_size(addr); // ask malloc for it's size.
      #else
      return malloc_usable_size(addr);
      #endif
    }
  }


  // get the start of the tialloc block from any pointer within it.
  small_block *get_tiblock_start(void *addr) {
    return (small_block *) ((int32_t)addr - (int32_t) addr%256);
  }

  // from a pointer to a data item in a tialloc block, return the data item index
  uint8_t ptr_to_data_idx(void *addr) {

    small_block *block_addr = get_tiblock_start(addr);

    return ((int32_t)addr)-(int32_t)(block_addr->data);
    //return addr-block_addr-11; 
  }

  bool any_free(small_block *addr) {

    if(addr == 0x0) return false; // nothing free!

    if((addr->free_chain_start == 0xFF) && (addr->free_chain_end == 0xFF)) return false;
    return true;
  }

  void free(void *addr) {

    if(!is_tiallocated(addr)) {::free(addr); return;}

    small_block *this_block = get_tiblock_start(addr);

    // are there existing free elements in this block?
    if(any_free(this_block)) {
      // there are existing free items

      //cout << "free: there are existing free items" << endl;
      uint8_t d_temp = this_block->free_chain_end;
      
      uint8_t free_item_idx = ptr_to_data_idx(addr);
      this_block->data[free_item_idx] = d_temp;

      this_block->free_chain_end = free_item_idx;
      return; 
    } else {
      // there are no existing free items

      //cout << "free: there are no existing free items" << endl;
      small_block *fb_temp = free_block_ptr[this_block->alloc_size];

      free_block_ptr[this_block->alloc_size] = get_tiblock_start(addr);

      this_block->free_chain_start = ptr_to_data_idx(addr);
      this_block->free_chain_end   = ptr_to_data_idx(addr);

      if(fb_temp != 0) fb_temp->next_free_block = this_block;
                  else free_block_ptr[this_block->alloc_size] = this_block; // I believe this is correct, select this block to start the chain.
      return;
    }
  }

  void *realloc(void *addr, int32_t new_size) {
   if(is_tiallocated(addr)) {

     cout << "new size: " << new_size << endl;
     cout << "addr   : " << (int) addr << endl;
     void *newaddr = alloc(new_size);
     int osize = alloc_size(addr);

     for(size_t n=0;n<osize;n++) {
       *((char *) newaddr+n) = *((char *) addr + n);
     }
     free(addr);
     return newaddr;

   } else {
     return ::realloc(addr,new_size);
   }
  }

  void *alloc(int32_t n_alloc_size=1) {

    if(n_alloc_size > max_tiallocation) {
      void *a = malloc(n_alloc_size);
      cout << "malloc: " << (int) a << endl;
      return a;
    }

    // are there any free items left?
    if(any_free(free_block_ptr[n_alloc_size])) {

      // only one item left?
      cout << "freeblock: " << (int) free_block_ptr[n_alloc_size] << endl;
      cout << "start: " << (int8_t) free_block_ptr[n_alloc_size]->free_chain_start << endl;
      cout << "end  : " << (int8_t) free_block_ptr[n_alloc_size]->free_chain_end << endl;
      cout << "endadr: " << (int) &(free_block_ptr[n_alloc_size]->free_chain_end) << endl;
      if(free_block_ptr[n_alloc_size]->free_chain_start == free_block_ptr[n_alloc_size]->free_chain_end) {
        cout << "alloc: only one item left" << endl;
        cout << "free_chain_start/end: " << (int) free_block_ptr[n_alloc_size]->free_chain_start << endl;
        int8_t ret_idx = free_block_ptr[n_alloc_size]->free_chain_end; // free_block_ptr->data[free_block_ptr->free_chain_end];
        free_block_ptr[n_alloc_size]->free_chain_start = 0xFF;
        free_block_ptr[n_alloc_size]->free_chain_end   = 0xFF;

        small_block *free_block_tmp = free_block_ptr[n_alloc_size]->prev_free_block;
        free_block_ptr[n_alloc_size]->prev_free_block  = 0;
        free_block_ptr[n_alloc_size]->next_free_block  = 0;

        void *retval = free_block_ptr[n_alloc_size]->data + ret_idx;
        free_block_ptr[n_alloc_size] = free_block_tmp;

        cout << "tialloc1: " << (int) retval << endl;
        return retval;
      } else {
        // more than one item left.
        cout << "alloc: more than one item left" << endl;

        uint8_t ret_idx = free_block_ptr[n_alloc_size]->free_chain_end;
        cout << "ret_idx: " << (int) ret_idx << endl;
        cout << "free_block_ptr: " << free_block_ptr[n_alloc_size] << endl;


        free_block_ptr[n_alloc_size]->free_chain_end = free_block_ptr[n_alloc_size]->data[free_block_ptr[n_alloc_size]->free_chain_end];

        void *retval = (void *) (((int32_t)free_block_ptr[n_alloc_size]->data) + ret_idx);
        cout << "tialloc2: " << (int) retval << endl;
        return retval;
      }


    } else {

      // allocate more blocks here
      cout << "reached block limit, allocate more" << endl;
      initialise(n_alloc_size);
      return alloc(n_alloc_size);
    }

  }

  void initialise(int n_alloc_size = 1) {

    m_alloc_size = 1024; //1024*1024*10;
    // malloc a bunch of memory

    // memory should really only be local, class member for debugging.
    uint8_t *memory = (uint8_t *)malloc(m_alloc_size); // 10Mb

    cout << "allocated: " << (small_block *) memory << endl;

    all_memory.push_back(memory);

    for(;((int32_t)memory)%256 !=0;) {memory++; m_alloc_size--; cout << "padding" << endl;} // pad allocation to be a multiple of 256
    cout << "padding complete" << endl;

    small_block t;
    t.prev_free_block  = 0;
    t.next_free_block  = 0;
    t.alloc_size       = n_alloc_size;
    t.free_chain_start = 0;
    t.free_chain_end   = 244 - (244%n_alloc_size);
    for(int n=n_alloc_size;n<245;n=n+n_alloc_size) {
      t.data[n] = n-n_alloc_size;
    }
    t.data[0] = 0xFF;
    cout << "built template" << endl;

    void *current_addr = memory;
    int n_limit = ((m_alloc_size/256)-1);
    cout << "n limit: " << n_limit << endl;
    for(int n=0;n<n_limit;n++) {
      // cout << "n: " << n << endl;
      t.prev_free_block = (small_block *)(((int32_t)current_addr)-256);
      t.next_free_block = (small_block *)(((int32_t)current_addr)+256);
      if(n==0) t.prev_free_block = 0;

      memcpy(current_addr,&t,256);

      current_addr = (void *) ((int32_t) current_addr + 256);
    }

    free_block_ptr[n_alloc_size] = (small_block *) ((int32_t) current_addr - 256);
    cout << "memory fill complete, initialisation complete" << endl;

  }

  void dump() {

    cout << "free_block_ptr[1]: " << free_block_ptr[1] << endl;

    cout << "Dump called" << endl;

    for(int i=0;i<all_memory.size();i++) {
      cout << "Dumping block: " << (small_block *) all_memory[i] << endl;
      for(int n=0;n<(m_alloc_size-256);n+=256) {
    
        cout << "Block address     : " << (small_block *)((int32_t)all_memory[i]+n) << endl;
        cout << "  next_free_block : " << ((small_block *)((int32_t)all_memory[i]+n))->next_free_block  << endl;
        cout << "  prev_free_block : " << ((small_block *)((int32_t)all_memory[i]+n))->prev_free_block  << endl;
        cout << "  free_chain_start: " << (int)((small_block *)((int32_t)all_memory[i]+n))->free_chain_start << endl;
        cout << "  free_chain_end  : " << (int)((small_block *)((int32_t)all_memory[i]+n))->free_chain_end   << endl;
        cout << "  alloc_size      : " << (int)((small_block *)((int32_t)all_memory[i]+n))->alloc_size       << endl;

        cout << "  data            : ";
        for(int32_t j=0;j<245;j++) { cout << (int)((small_block *)((int32_t)all_memory[i]+n))->data[j] << ","; }
        cout << endl;
      }
    }
  }

};

#endif
