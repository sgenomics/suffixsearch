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
 
 
      if(0 > ConnectFD) {
        perror("error accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }

      char buf[201];

      string search_string;
      bool notfound=true;

      string endstr;
      endstr.push_back((char) 13);
      endstr.push_back((char) 10);
      endstr.push_back((char) 13);
      endstr.push_back((char) 10);
      for(int n=0;;n++) {
        int cnt = read(ConnectFD, buf, 200);

        // parse out search string
        buf[200] = 0;
        string s = buf;
        size_t getline = s.find(endstr);
        size_t start = s.find("arch?q=");
        size_t end   = s.find("&",start);
        if(start != string::npos) {notfound=false; search_string = s.substr(start+7,end-start-7);}
        if(getline != string::npos) { break; }
        if(n == 100) break;
      }

      string output_data;
      if(notfound==false) {
        vector<char> ss;
        for(int n=0;n<search_string.size();n++) if(search_string[n] == '+') search_string[n] = ' ';
        cout << "search_string: " << search_string << endl;
        for(int n=0;n<search_string.size();n++) ss.push_back(search_string[n]); 

        bool found=false;
        vector<int> foundpos = m_store.all_occurs(ss);
        cout << "found count: " << foundpos.size() << endl;
        if(foundpos.size() > 0) found=true;

        if(found) {
          for(int i=0;(i<foundpos.size()) && (i<100);i++) {
            cout << "found location: " << foundpos[i] << endl;
            string fnd = m_store.get_substr(foundpos[i],foundpos[i]+50);
            output_data += fnd;

            output_data += "<BR>";
          }
          cout << "fin write" << endl;
        }
        if(!found) output_data = "NOTFOUND"; 
      } else {
        output_data = "INCORRECT SEARCH FORMAT\n\n";
      }

      char data[20000];
      strcpy(data,"HTTP/1.0 200\n");
      int val = write(ConnectFD,(void *) data,strlen(data));
      strcpy(data,"Content-Type: text/html\n");
      val = write(ConnectFD,(void *) data,strlen(data));
      strcpy(data,"Content-Length: 11\n\n\n");
      val = write(ConnectFD,(void *) data,strlen(data));

      strcpy(data,output_data.c_str());
      val = write(ConnectFD,(void *) data,strlen(data));

      close(ConnectFD);
    }
    close(SocketFD);

  }

  SuffixTree &m_store;
};

#endif
