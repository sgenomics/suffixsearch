
insert: insert.cpp SuffixTree.h SuffixNode.h SuffixNodeStore.h SuffixNodeStore.cpp
	g++ -g insert.cpp ./SuffixNode.cpp ./SuffixNodeStore.cpp -o insert
insertfile: insertfile.cpp SuffixTree.h SuffixNode.h SuffixNodeStore.h SuffixNodeStore.cpp *.h *.cpp
	g++ -g insertfile.cpp tialloc.cpp ./SuffixNode.cpp ./SuffixNodeStore.cpp -o insertfile
web: tialloc.h tialloc.cpp web.cpp SuffixTree.h WebSrvSearch.h Transcode.h ChildStore.h SuffixNode.h ObjectStoreMem.h ObjectStoreDisk.h ObjectStoreMemVec.h SuffixNodeStore.h URIStore.h URIStore.cpp global_defs.h
	g++ -O3 -Wall web.cpp ./tialloc.cpp ./SuffixNode.cpp ./SuffixNodeStore.cpp ./URIStore.cpp -o web

searchmark: SuffixTree.h Transcode.h ChildStore.h SuffixNode.h
	g++ -O3 searchmark.cpp ./SuffixNode.cpp ./SuffixNodeStore.cpp -o searchmark -m32 -L/usr/lib32

malmark: malmark.cpp tialloc.cpp tialloc.h
	g++ -O3 -march=native malmark.cpp tialloc.cpp -o malmark

clean:
	rm insert web insertfile
