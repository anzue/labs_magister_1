#ifndef SHA_224_H
#define SHA_224_H
#include <string>
using std::string;
namespace SHA2 {
    void get_word_hash(unsigned int* w);
    void print_arr(unsigned int* arr ,int size = 1, bool hex = false);
    string print_res(bool hex=false);
    string SHA2_main(string s);
}
#endif // SHA_224_H
