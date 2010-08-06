
insert: insert.cpp SuffixTree.h
	g++ -O3 insert.cpp ./SuffixTree.cpp -o insert
web: web.cpp SuffixTree.h WebSrvSearch.h
	g++ -O3 web.cpp ./SuffixTree.cpp -o web

clean:
	rm insert web
