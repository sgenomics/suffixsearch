#ifndef WebSrvSEARCH
#define WebSrvSEARCH

#include <sys/types.h>
#include <sys/socket.h>
#include <vector>
#include <string.h>
#include <stdio.h>
#include "stringify.h"
#include "URIStore.h"
using namespace std;

#include <netinet/in.h>
#include <signal.h>

class WebSrvSearch {

public:
  WebSrvSearch(SuffixTree &store,int search_port,int document_port) : m_store(store),m_search_port(search_port),m_document_port(document_port) {
  }

  void start() {
    search_listener();
    document_listener();
  }

  void document_listener() {

    signal(SIGPIPE,SIG_IGN);

    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
 
    if(-1 == SocketFD) {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(m_document_port);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in))) {
      perror("error bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    if(-1 == listen(SocketFD, 10)) {
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

      string uri_string;
      bool notfound=true;

      string endstr;
      endstr.push_back((char) 13);
      endstr.push_back((char) 10);
      endstr.push_back((char) 13);
      endstr.push_back((char) 10);

      string document;

      //TODO: this needs fixing max uri size is 200.
      bool uri_unset=true;
      for(int n=0;;n++) {
        int read_size = read(ConnectFD, buf, 200);

        if(read_size < 1) break;

        if(uri_unset) {
	  // parse out uri_unset string
	  //buf[200] = 0;
          buf[read_size] = 0;
	  string s = buf;
	  size_t getline = s.find(endstr);
	  size_t start = s.find("uri=");
	  size_t end   = s.find("\n",start);
	  if(start != string::npos) {notfound=false; uri_string = s.substr(start+4,end-start-4);}
	  if(getline != string::npos) { break; }
          uri_unset=false;

          //TODO: must modify to deal with documents larger than memory
          document += s.substr(end+1,string::npos);
        } else {
          if(read_size < 1) break;
	  buf[200] = 0;
          if(read_size < 200) buf[read_size+1] = 0;
          document += buf;
        }
      }

      // Send 200 OK
      char data[20000];
      strcpy(data,"HTTP/1.0 200\n");
      int val = write(ConnectFD,(void *) data,strlen(data));
      strcpy(data,"Content-Type: text/html\n");
      val = write(ConnectFD,(void *) data,strlen(data));
      strcpy(data,(string("Content-Length: 0\n\n\n").c_str()));
      val = write(ConnectFD,(void *) data,strlen(data));

      size_t document_start = m_store.size();

      cout << "document uri: " << uri_string << endl;
      cout << "document    : " << document << endl;

      if(uri_string.compare("END") == 0) break;
      m_uri_store.insert(document_start,uri_string);
      m_store.insert(document);

      close(ConnectFD);

    }
    close(SocketFD);
  }

  string remove_nonalpha(string str) {

    string outstr;
    for(size_t n=0;n<str.size();n++) {
      bool ok = false;
      if(str[n] >= 'a' && str[n] <= 'z') ok = true;
      if(str[n] >= 'A' && str[n] <= 'Z') ok = true;

      if(ok == true ) outstr += str[n];
      if(ok == false) outstr += "&nbsp;";
    }

    return outstr;
  }

  void search_listener() {

    signal(SIGPIPE,SIG_IGN);

    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
 
    if(-1 == SocketFD) {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(m_search_port);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in))) {
      perror("error bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    if(-1 == listen(SocketFD, 10)) {
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

      // this needs fixing, max search string size is 200.
      for(int n=0;;n++) {
        read(ConnectFD, buf, 200);

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
      output_data += "<tt>";
      if(notfound==false) {
        vector<char> ss;
        for(size_t n=0;n<search_string.size();n++) if(search_string[n] == '+') search_string[n] = ' ';
        cout << "search_string: " << search_string << endl;
        for(size_t n=0;n<search_string.size();n++) ss.push_back(search_string[n]); 

        bool found=false;
        vector<int> foundpos = m_store.all_occurs(ss,1000);
        cout << "found count: " << foundpos.size() << endl;
        if(foundpos.size() > 0) found=true;

        if(found) {
          for(size_t i=0;(i<foundpos.size()) && (i<1000);i++) {
            cout << "found location: " << foundpos[i] << endl;
            string fnd = m_store.get_substr(foundpos[i]-50,foundpos[i]-1);
            if(fnd.size() < 50) {
              int spacers = 50-fnd.size();
              string spacer_string;
              for(size_t n=0;n<spacers;n++) {
                spacer_string += '_';
              }
              fnd = spacer_string + fnd;
            }
            fnd = remove_nonalpha(fnd);
            cout << "fnd1 size: " << fnd.size() << endl;
            output_data += fnd;
            output_data += "<B>";
            fnd = m_store.get_substr(foundpos[i],foundpos[i]+search_string.size()-1);
            fnd = remove_nonalpha(fnd);
            output_data += fnd;
            output_data += "</B>";
            fnd = m_store.get_substr(foundpos[i]+search_string.size(),foundpos[i]+50);
            if(fnd.size() < 50) {
              int spacers = 50-fnd.size();
              string spacer_string;
              for(size_t n=0;n<spacers;n++) {
                spacer_string += '_';
              }
              fnd = fnd + spacer_string;
            }
            fnd = remove_nonalpha(fnd);
            cout << "fnd2 size: " << fnd.size() << endl;
            output_data += fnd;

            output_data += "<A HREF=\"" + m_uri_store.find_bounding(foundpos[i]) + "\">URI</A>";

            output_data += "<BR>";
          }
          cout << "fin write" << endl;
        }
        if(!found) output_data = "NOTFOUND"; 
      } else {
        output_data = "INCORRECT SEARCH FORMAT\n\n";
      }
      output_data += "</tt>";

      char data[20000];
      strcpy(data,"HTTP/1.0 200\n");
      int val = write(ConnectFD,(void *) data,strlen(data));
      strcpy(data,"Content-Type: text/html\n");
      val = write(ConnectFD,(void *) data,strlen(data));
      strcpy(data,(string("Content-Length: ") + stringify(output_data.size()) + "\n\n\n").c_str());
      val = write(ConnectFD,(void *) data,strlen(data));

      val = write(ConnectFD,(void *) output_data.c_str(),output_data.size());

      close(ConnectFD);
    }
    close(SocketFD);
  }

  SuffixTree &m_store;
  URIStore    m_uri_store;
  int         m_search_port;
  int         m_document_port;
};

#endif
