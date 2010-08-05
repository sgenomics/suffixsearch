#ifndef WebSrvSEARCH
#define WebSrvSEARCH

#include <sys/types.h>
#include <sys/socket.h>
#include <vector>
#include <string.h>
#include <stdio.h>
using namespace std;

#include <netinet/in.h>




class WebSrvSearch {

public:
  WebSrvSearch(SuffixTree &store,int port) : m_store(store) {
    
  } 

  void start() {

    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
 
    if(-1 == SocketFD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("error bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    if(-1 == listen(SocketFD, 10))
    {
      perror("error listen failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    for(;;)
    {
      int ConnectFD = accept(SocketFD, NULL, NULL);
 
 
      if(0 > ConnectFD)
      {
        perror("error accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }

      char buf[201];
      int cnt = read(ConnectFD, buf, 200);

      // parse out search string
      buf[200] = 0;
      string s = buf;
      size_t start = s.find("search=");
      size_t end   = s.find("0");
      string search_string = s.substr(start+7,end-start-7);
      cout << "search_string: " << search_string << endl;

      vector<char> ss;
      for(int n=0;n<search_string.size();n++) ss.push_back(search_string[n]);
      bool found = m_store.exists(ss);
 
      /* perform read write operations ... */
      char data[20];
      if(found ) strcpy(data,"FOUND");
      if(!found) strcpy(data,"NOTFOUND");
      int val = write(ConnectFD,(void *) data,10);
      cout << found << endl; 

      close(ConnectFD);
    }
 
    close(SocketFD);

  }

  SuffixTree &m_store;
};

#endif
