
insert: insert.cpp SuffixTree.h SuffixNode.h SuffixNodeStore.h
	g++ -g insert.cpp ./SuffixNode.cpp -o insert
web: web.cpp SuffixTree.h WebSrvSearch.h Transcode.h ChildStore.h SuffixNode.h
	g++ -O3 web.cpp ./SuffixNode.cpp -o web

clean:
	rm insert web
