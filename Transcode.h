#ifndef TRANSCODE
#define TRANSCODE

#include <vector>

#include <stdio.h>
#include <ctype.h>

using namespace std;


///\brief transcoding module. Converts ascii to limited character set.
class Transcode {

public:

  vector<char> table;

  Transcode() {
    table = vector<char>(256,37);

    table['A'] = 0;
    table['B'] = 1;
    table['C'] = 2;
    table['D'] = 3;
    table['E'] = 4;
    table['F'] = 5;
    table['G'] = 6;
    table['H'] = 7;
    table['I'] = 8;
    table['J'] = 9;
    table['K'] = 10;
    table['L'] = 11;
    table['M'] = 12;
    table['N'] = 13;
    table['O'] = 14;
    table['P'] = 15;
    table['Q'] = 16;
    table['R'] = 17;
    table['S'] = 18;
    table['T'] = 19;
    table['U'] = 20;
    table['V'] = 21;
    table['W'] = 22;
    table['X'] = 23;
    table['Y'] = 24;
    table['Z'] = 25;
    table['0'] = 26;
    table['1'] = 27;
    table['2'] = 28;
    table['3'] = 29;
    table['4'] = 30;
    table['5'] = 31;
    table['6'] = 32;
    table['7'] = 33;
    table['8'] = 34;
    table['9'] = 35;
    table[' '] = 36;
    table[9]  = 36;
    table[10] = 36;
    table[13] = 36;

    table['a'] = 0;
    table['b'] = 1;
    table['c'] = 2;
    table['d'] = 3;
    table['e'] = 4;
    table['f'] = 5;
    table['g'] = 6;
    table['h'] = 7;
    table['i'] = 8;
    table['j'] = 9;
    table['k'] = 10;
    table['l'] = 11;
    table['m'] = 12;
    table['n'] = 13;
    table['o'] = 14;
    table['P'] = 15;
    table['q'] = 16;
    table['r'] = 17;
    table['s'] = 18;
    table['t'] = 19;
    table['u'] = 20;
    table['v'] = 21;
    table['w'] = 22;
    table['x'] = 23;
    table['y'] = 24;
    table['z'] = 25;
  }

  char convert(unsigned char in) {
    return table[in];
  }

};

#endif
