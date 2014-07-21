all: build_disk_idx

build_disk_idx: 
	g++ -fopenmp -O3 -Wall build_disk_idx.cpp ../suffixcore/tialloc/tialloc.cpp ../suffixcore/SuffixNode.cpp ../URIStore.cpp -I../suffixcore -I../suffixtrans -I../suffixcore/tialloc ../suffixcore/SuffixNodeStoreMemVec.cpp ../suffixcore_optional/SuffixNodeStoreDisk.cpp -o build_disk_idx -I.. -I../suffixcore_optional -I../suffixcore -I. -lz 

