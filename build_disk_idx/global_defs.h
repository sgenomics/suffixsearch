/* -
 * Copyright (c) 2012 Nava Whiteford <new@sgenomics.org>
 * suffixcore - core suffixtree algorithms
 *
 * A license to use this software is granted to users given access to the git repository at: https://github.com/sgenomics/suffixcore
 * A complete copy of this license is located in the license.txt file of this package.
 *
 * In Summary this software:
 *
 * Can be used for creating unlimited applications.
 * Can be distributed in binary or object form only.
 * Commercial use is allowed.
 * Can modify source-code but cannot distribute modifications (derivative works).
 */

#ifndef GLOBALDEFS
#define GLOBALDEFS

#define suffixnodestore_type SuffixNodeStoreMemVec

//define one of these to delect a storage mode
#define object_store_type ObjectStoreMemVec

//define one of these to select a memory allocator
#define use_tialloc 1

#define alphabet_size  65534
#define symbol_type uint16_t
#define symbol_size  65534
#define final_symbol 65535

#define store_type vector<symbol_type>
#include "SuffixNodeStoreMemVec.h"

#endif
