web: ../suffixcore/tialloc/tialloc.h ../suffixcore/tialloc/tialloc.cpp web.cpp ../suffixcore/SuffixTree.h ../WebSrvSearch.h ../suffixcore/SuffixNode.h ../URIStore.h ../URIStore.cpp  
	g++ -O3 -Wall -I. -I.. web.cpp ../suffixcore/tialloc/tialloc.cpp ../suffixcore/SuffixNode.cpp ../URIStore.cpp -I../suffixcore -I../suffixtrans -I../suffixcore/tialloc ../suffixcore/SuffixNodeStoreMemVec.cpp -I.. -o web 

clean:
	rm web
