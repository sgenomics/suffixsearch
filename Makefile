
insert: insert.cpp SuffixTree.h SuffixNode.h SuffixNodeStore.h SuffixNodeStore.cpp
	g++ -g insert.cpp ./SuffixNode.cpp ./SuffixNodeStore.cpp -o insert

insertfile: insertfile.cpp SuffixTree.h SuffixNode.h SuffixNodeStore.h SuffixNodeStore.cpp *.h *.cpp
	g++ -O3 insertfile.cpp tialloc.cpp ./SuffixNode.cpp ./SuffixNodeStore.cpp -o insertfile

web: ./suffixcore/tialloc/tialloc.h ./suffixcore/tialloc/tialloc.cpp web.cpp ./suffixcore/SuffixTree.h WebSrvSearch.h Transcode.h ./suffixcore_optional/ChildStore.h ./suffixcore/SuffixNode.h ./suffixcore_optional/ObjectStoreMem.h ./suffixcore_optional/ObjectStoreDisk.h ./suffixcore_optional/ObjectStoreMemVec.h ./suffixcore_optional/SuffixNodeStore.h URIStore.h URIStore.cpp global_defs.h 
	g++ -O3 -Wall web.cpp ./suffixcore/tialloc/tialloc.cpp ./suffixcore/SuffixNode.cpp ./suffixcore_optional/SuffixNodeStore.cpp ./URIStore.cpp -I./suffixcore -I./suffixcore_optional -I./suffixcore/tialloc -o web

searchmark: SuffixTree.h Transcode.h ChildStore.h SuffixNode.h
	g++ -O3 searchmark.cpp ./SuffixNode.cpp ./SuffixNodeStore.cpp ./tialloc.cpp -o searchmark

searchmarkcur: SuffixTree.h Transcode.h ChildStore.h SuffixNode.h
	g++ -O3 searchmarkcur.cpp ./SuffixNode.cpp ./SuffixNodeStore.cpp ./tialloc.cpp -o searchmark

aligner: SuffixTree.h Transcode.h ChildStore.h SuffixNode.h aligner.cpp
	g++ -O3 aligner.cpp ./SuffixNode.cpp ./SuffixNodeStore.cpp ./tialloc.cpp -o aligner

malmark: malmark.cpp tialloc.cpp tialloc.h
	g++ -O3 -march=native malmark.cpp tialloc.cpp -o malmark

clean:
	rm insert web insertfile
