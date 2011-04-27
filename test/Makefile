
test: test_main.cpp ../SuffixTree.h ../SuffixNode.h test_suffixtree_simple.cpp test_suffixtree_random.cpp test_suffixtree_time.cpp test_suffixtree_alloccurs.cpp test_suffixtree_suffixnodestore.cpp test_suffixtree_suffixnodestore.h
	#~/x86_open64-4.2.4/bin/openCC -mtune=opteron -march=opteron -Wall -g -O3 test_main.cpp test_suffixtree_alloccurs.cpp test_suffixtree_processpositions.cpp test_suffixtree_simple.cpp test_suffixtree_random.cpp test_suffixtree_time.cpp test_objectstore.cpp test_suffixtree_suffixnodestore.cpp ../SuffixNode.cpp ../SuffixNodeStore.cpp -o test -I..
	g++ -Wall -g test_main.cpp test_suffixtree_alloccurs.cpp test_suffixtree_processpositions.cpp test_suffixtree_simple.cpp test_suffixtree_random.cpp test_suffixtree_time.cpp test_objectstore.cpp ../SuffixNode.cpp ../SuffixNodeStore.cpp -o test -I..
#	g++ -Wall -g -O3 test_main.cpp test_suffixtree_alloccurs.cpp test_suffixtree_processpositions.cpp test_suffixtree_simple.cpp test_suffixtree_random.cpp test_suffixtree_time.cpp test_objectstore.cpp test_suffixtree_suffixnodestore.cpp ../SuffixNode.cpp ../SuffixNodeStore.cpp -o test -I..
	./test
#-fno-strict-aliasing -fno-strict-overflow -fno-strict-aliasing -
clean:
	rm test
