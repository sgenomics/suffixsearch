#ifndef TRANSCODE
#define TRANSCODE

#include <vector>

#include <stdio.h>
#include <ctype.h>

using namespace std;


///\brief transcoding module. Converts ascii to limited character set.
class Transcode {

public:
  Transcode() {
  }

  char convert(char in) {
    char i = toupper(in);
    // Here comes the twister
    if(i == 'A') return 0;
    if(i == 'B') return 1;
    if(i == 'C') return 2;
    if(i == 'D') return 3;
    if(i == 'E') return 4;
    if(i == 'F') return 5;
    if(i == 'G') return 6;
    if(i == 'H') return 7;
    if(i == 'I') return 8;
    if(i == 'J') return 9;
    if(i == 'K') return 10;
    if(i == 'L') return 11;
    if(i == 'M') return 12;
    if(i == 'N') return 13;
    if(i == 'O') return 14;
    if(i == 'P') return 15;
    if(i == 'Q') return 16;
    if(i == 'R') return 17;
    if(i == 'S') return 18;
    if(i == 'T') return 19;
    if(i == 'U') return 20;
    if(i == 'V') return 21;
    if(i == 'W') return 22;
    if(i == 'X') return 23;
    if(i == 'Y') return 24;
    if(i == 'Z') return 25;
    if(i == '0') return 26;
    if(i == '1') return 27;
    if(i == '2') return 28;
    if(i == '3') return 29;
    if(i == '4') return 30;
    if(i == '5') return 31;
    if(i == '6') return 32;
    if(i == '7') return 33;
    if(i == '8') return 34;
    if(i == '9') return 35;
    if(i == ' ') return 36;
    if(i == 9  ) return 36; // tab
    if(i == 10 ) return 36; // lf
    if(i == 13 ) return 36; // cr
    return 37;
  }

};

#endif
