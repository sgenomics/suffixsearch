
simple: simple.cpp SuffixTree.h
	g++ -g simple.cpp -o simple
insert: insert.cpp SuffixTree.h
	g++ -O3 insert.cpp ./SuffixTree.cpp -o insert
suffixtree: suffixtree.cpp SuffixTree.h
	g++ -O3 suffixtree.cpp -o suffixtree

all: simple insert suffixtree

clean:
	rm simple insert suffixtree
