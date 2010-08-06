
test: test_main.cpp ../SuffixTree.h test_suffixtree_simple.cpp test_suffixtree_random.cpp test_suffixtree_time.cpp
	g++ -O3 test_main.cpp test_suffixtree_processpositions.cpp test_suffixtree_simple.cpp test_suffixtree_random.cpp test_suffixtree_time.cpp ../SuffixTree.cpp -o test -I..
	./test

clean:
	rm test
