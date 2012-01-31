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


using namespace std;

struct small_block {
  small_block *next_free_block;
  small_block *prev_free_block;
  uint8_t         free_chain_start;   // 0xFF means unallocated, otherwise index in to data.
  uint8_t         free_chain_end;     // 0xFF means unallocated, otherwise index in to data.
  uint8_t         alloc_size;

  #ifdef ENV32BIT
  uint8_t         data[245];
  #endif
  #ifdef ENV64BIT
  uint8_t         data[237];
  #endif
} __attribute__((__packed__));

class tialloc {

  small_block *free_block_ptr[256]; // pointers to last_free_blocks
  intarch_t m_alloc_size;
  vector<vector<uint8_t *> > all_memory;    // this isn't required, and should be removed from non-debug implementations.
  int max_tiallocation;

  
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

    small_block *this_block = get_tiblock_start(addr);
//    if(this_block->alloc_size > max_tiallocation) return false;
    for(int allocs = 1;allocs <= max_tiallocation;allocs++) 
    for(int n=0;n < all_memory[allocs].size();n++) {
    //for(int n=0;n < all_memory[this_block->alloc_size].size();n++) { // would rather use this...
      // int allocs = this_block->alloc_size;

      #ifdef __APPLE__
      int size = malloc_size(all_memory[allocs][n]); // ask malloc for it's size.
      #else
      int size = malloc_usable_size(all_memory[allocs][n]);
      #endif

      int block_start = (int) all_memory[allocs][n];
      int block_end   = (int) all_memory[allocs][n] + size;
      int thisb       = (int) addr;

      if((thisb > block_start) && (thisb < block_end)) { return true;}

//      if(((int)this_block >= (int)all_memory[allocs][n]) && ((int)this_block <= ((int)all_memory[allocs][n] + size))) return true;
    }
    return false;


    // Previous method, retained for reference, however malloc_usable_size can segfault
    if(((intarch_t)addr)%4 == 0) {  // all malloc'd blocks will be word aligned.
      void *aligned_addr = (void *)(((intarch_t)addr) - (intarch_t)addr%4);

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

  intarch_t alloc_size(void *addr) {
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
    return (small_block *) ((intarch_t)addr - (intarch_t) addr%256);
  }

  // from a pointer to a data item in a tialloc block, return the data item index
  uint8_t ptr_to_data_idx(void *addr) {

    small_block *block_addr = get_tiblock_start(addr);

    return ((intarch_t)addr)-(intarch_t)(block_addr->data);
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

  void *realloc(void *addr, intarch_t new_size) {
   if(is_tiallocated(addr)) {

     void *newaddr = alloc(new_size);
     int osize = alloc_size(addr);

     for(size_t n=0;n<osize;n++) {
       *(((char *) newaddr)+n) = *(((char *) addr) + n);
     }
     free(addr);
     return newaddr;

   } else {
     return ::realloc(addr,new_size);
   }
  }

  void *alloc(intarch_t n_alloc_size=1) {

    if(n_alloc_size == 0) return 0;

    if(n_alloc_size > max_tiallocation) {
      void *a = malloc(n_alloc_size);
      return a;
    }

    // are there any free items left?
    if(any_free(free_block_ptr[n_alloc_size])) {

      // only one item left?
      if(free_block_ptr[n_alloc_size]->free_chain_start == free_block_ptr[n_alloc_size]->free_chain_end) {
        uint8_t ret_idx = free_block_ptr[n_alloc_size]->free_chain_end; // free_block_ptr->data[free_block_ptr->free_chain_end];
        free_block_ptr[n_alloc_size]->free_chain_start = 0xFF;
        free_block_ptr[n_alloc_size]->free_chain_end   = 0xFF;

        small_block *free_block_tmp = free_block_ptr[n_alloc_size]->prev_free_block;
        free_block_ptr[n_alloc_size]->prev_free_block  = 0;
        free_block_ptr[n_alloc_size]->next_free_block  = 0;

        void *retval = (void *) (((char *) free_block_ptr[n_alloc_size]->data) + ret_idx);
        free_block_ptr[n_alloc_size] = free_block_tmp;

        return retval;
      } else {
        // more than one item left.



        uint8_t ret_idx = free_block_ptr[n_alloc_size]->free_chain_end;


        free_block_ptr[n_alloc_size]->free_chain_end = free_block_ptr[n_alloc_size]->data[free_block_ptr[n_alloc_size]->free_chain_end];

        void *retval = (void *) (((intarch_t)free_block_ptr[n_alloc_size]->data) + ret_idx);
        return retval;
      }


    } else {

      // allocate more blocks here
      initialise(n_alloc_size);
      return alloc(n_alloc_size);
    }

  }

  void initialise(int n_alloc_size = 1) {

    m_alloc_size = 1024*10; //1024*1024*10;
    // malloc a bunch of memory

    // memory should really only be local, class member for debugging.
    uint8_t *memory = (uint8_t *)malloc(m_alloc_size); // 10Mb

    all_memory[n_alloc_size].push_back(memory);

    for(;((intarch_t)memory)%256 !=0;) {memory++; m_alloc_size--;} // pad allocation to be a multiple of 256

    small_block t;
    t.prev_free_block  = 0;
    t.next_free_block  = 0;
    t.alloc_size       = n_alloc_size;
    t.free_chain_start = 0;
    t.free_chain_end   = 244 - (244%n_alloc_size) - n_alloc_size; // added - n_alloc_size
    for(int n=n_alloc_size;n<245;n=n+n_alloc_size) {
      t.data[n] = n-n_alloc_size;
    }
    t.data[0] = 0xFF;

    void *current_addr = memory;
    int n_limit = ((m_alloc_size/256)-1); // should be -1?
    for(int n=0;n<n_limit;n++) {
      t.prev_free_block = (small_block *)(((intarch_t)current_addr)-256);
      t.next_free_block = (small_block *)(((intarch_t)current_addr)+256);
      if(n==0) t.prev_free_block = 0;

      memcpy(current_addr,&t,256);

      current_addr = (void *) ((intarch_t) current_addr + 256);
    }

    free_block_ptr[n_alloc_size] = (small_block *) ((intarch_t) current_addr - 256);

  }

  void dump() {

    cout << "free_block_ptr[1]: " << free_block_ptr[1] << endl;

    cout << "Dump called" << endl;

    for(int j=0;j<all_memory.size();j++)
    for(int i=0;i<all_memory[j].size();i++) {
      cout << "Dumping block: " << (small_block *) all_memory[j][i] << endl;
      for(int n=0;n<(m_alloc_size-256);n+=256) {
    
        cout << "Block address     : " << (small_block *)((intarch_t)all_memory[j][i]+n) << endl;
        cout << "  next_free_block : " << ((small_block *)((intarch_t)all_memory[j][i]+n))->next_free_block  << endl;
        cout << "  prev_free_block : " << ((small_block *)((intarch_t)all_memory[j][i]+n))->prev_free_block  << endl;
        cout << "  free_chain_start: " << (int)((small_block *)((intarch_t)all_memory[j][i]+n))->free_chain_start << endl;
        cout << "  free_chain_end  : " << (int)((small_block *)((intarch_t)all_memory[j][i]+n))->free_chain_end   << endl;
        cout << "  alloc_size      : " << (int)((small_block *)((intarch_t)all_memory[j][i]+n))->alloc_size       << endl;

        cout << "  data            : ";
        for(intarch_t k=0;k<245;k++) { cout << (int)((small_block *)((intarch_t)all_memory[j][i]+n))->data[k] << ","; }
        cout << endl;
      }
    }
  }

};

#endif
