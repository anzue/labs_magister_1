#ifndef SHA_224_H
#define SHA_224_H

#include "defines.h"
#include <iomanip>

using namespace std;

namespace SHA2 {

const int word_count =16;
const int total_count = 64;

void get_word_hash(unsigned int* w);
void print_arr(unsigned int* arr ,int size/* = 1*/, bool hex/* = false*/);

string print_res(bool hex/*=false*/);
string SHA2_main(string s);
}
#endif // SHA_224_H
