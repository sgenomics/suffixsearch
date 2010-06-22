
simple: simple.cpp SuffixTree.h
	g++ -g simple.cpp -o simple
insert: insert.cpp SuffixTree.h
	g++ -g insert.cpp -o insert
suffixtree: suffixtree.cpp SuffixTree.h
	g++ -O3 suffixtree.cpp -o suffixtree

all: simple insert suffixtree

clean:
	rm simple insert suffixtree
