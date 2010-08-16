
insert: insert.cpp SuffixTree.h SuffixNode.h SuffixNodeStore.h
	g++ -g insert.cpp ./SuffixNode.cpp -o insert
web: web.cpp SuffixTree.h WebSrvSearch.h Transcode.h ChildStore.h SuffixNode.h
	g++ -O3 web.cpp ./SuffixNode.cpp ./SuffixNodeStore.cpp  -o web -m32 -L/usr/lib32

searchmark: SuffixTree.h Transcode.h ChildStore.h SuffixNode.h
	g++ -O3 searchmark.cpp ./SuffixNode.cpp -o searchmark -m32 -L/usr/lib32

clean:
	rm insert web
