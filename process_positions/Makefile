all: process_positions

process_positions: 
	g++ -g -O3 -Wall process_positions.cpp ../suffixcore/tialloc/tialloc.cpp ../suffixcore/SuffixNode.cpp ../URIStore.cpp -I../suffixcore -I../suffixtrans -I../suffixcore/tialloc ../suffixcore/SuffixNodeStoreMemVec.cpp ../suffixcore_optional/SuffixNodeStoreDisk.cpp -o process_positions -I.. -I../suffixcore_optional -I../suffixcore -I.
