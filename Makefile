
insert: insert.cpp SuffixTree.h
	g++ -g insert.cpp ./SuffixTree.cpp -o insert
web: web.cpp SuffixTree.h WebSrvSearch.h Transcode.h
	g++ -g web.cpp ./SuffixTree.cpp -o web

clean:
	rm insert web
