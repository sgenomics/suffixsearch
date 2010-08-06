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
 
    for(;;) {
      int ConnectFD = accept(SocketFD, NULL, NULL);
 
 
      if(0 > ConnectFD)
      {
        perror("error accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }

      char buf[201];

      string search_string;
      bool notfound=true;
      for(int n=0;notfound;n++) {
        int cnt = read(ConnectFD, buf, 200);

        // parse out search string
        buf[200] = 0;
        string s = buf;
        size_t start = s.find("search=");
        size_t end   = s.find("&");
        search_string = s.substr(start+7,end-start-7);
        if(start != string::npos) cout << "search_string: " << search_string << endl;
        if(start == string::npos) notfound = true; else notfound = false;
        if(n == 10) break;
        cout << "process loop" << endl;
      }

      if(notfound==false) {
        vector<char> ss;
        for(int n=0;n<search_string.size();n++) ss.push_back(search_string[n]);
        bool found = m_store.exists(ss);

        char data[2000];

        strcpy(data,"HTTP/1.0 200 \n");
        int val = write(ConnectFD,(void *) data,strlen(data));
        strcpy(data,"Content-Type: text/html\n");
        val = write(ConnectFD,(void *) data,strlen(data));
        strcpy(data,"Content-Length: 8\n\n\n");
        val = write(ConnectFD,(void *) data,strlen(data));
 
        /* perform read write operations ... */
        if(found ) strcpy(data,"__FOUND_");
        if(!found) strcpy(data,"NOTFOUND");
        val = write(ConnectFD,(void *) data,8);
        cout << found << endl; 
      }
      close(ConnectFD);
    }
 
    close(SocketFD);

  }

  SuffixTree &m_store;
};

#endif
