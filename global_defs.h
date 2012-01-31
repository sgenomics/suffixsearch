#ifndef GLOBALDEFS
#define GLOBALDEFS

#define suffixnodestore_type SuffixNodeStoreMemVec
//#define suffixnodestore_type SuffixNodeStore

//#define object_store_type ObjectStoreMem
#define object_store_type ObjectStoreMemVec
//#define object_store_type ObjectStoreDisk

//#define use_malloc 1
#define use_tialloc 1

#define alphabet_size  40
#define symbol_size  41
#define final_symbol 39

#endif
