
test: test_main.cpp ../SuffixTree.h ../SuffixNode.h test_suffixtree_simple.cpp test_suffixtree_random.cpp test_suffixtree_time.cpp test_suffixtree_alloccurs.cpp test_suffixtree_suffixnodestore.cpp test_suffixtree_suffixnodestore.h ./test_tialloc.cpp ../tialloc.h
	g++ -Wall -g test_tialloc.cpp test_main.cpp test_suffixtree_alloccurs.cpp test_suffixtree_processpositions.cpp test_suffixtree_simple.cpp test_suffixtree_random.cpp test_suffixtree_time.cpp test_objectstore.cpp ../SuffixNode.cpp ../SuffixNodeStore.cpp ../tialloc.cpp -o test -I..
clean:
	rm test
