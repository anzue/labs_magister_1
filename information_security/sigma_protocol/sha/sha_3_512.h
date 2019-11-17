#ifndef SHA_3_512_H
#define SHA_3_512_H
#include "defines.h"

typedef unsigned long long word;

using namespace std;

namespace SHA3 {
    void get_word_hash(word* data);
    void print_arr(word* arr ,int size = 1, bool hex = false);
    void print_res(bool hex=false);
     void reset_vars();
    string SHA3_main(string s);
}
#endif // SHA_3_512_H
