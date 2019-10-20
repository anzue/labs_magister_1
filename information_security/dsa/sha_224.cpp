#include "defines.h"
#include <iomanip>
#define shr(x,a) (x>>a)
#define rotr(x,a) ((x>>a) | (x << (32 - a)) )
#define asbits(x) (std::bitset<8*sizeof(int)>(x))

using namespace std;

namespace SHA2 {

//for 256

/*
unsigned int
          h0 = 0x6A09E667,
          h1 = 0xBB67AE85,
          h2 = 0x3C6EF372,
          h3 = 0xA54FF53A,
          h4 = 0x510E527F,
          h5 = 0x9B05688C,
          h6 = 0x1F83D9AB,
          h7 = 0x5BE0CD19;


const int word_count =16;
const int total_count = 64;
*/


unsigned int
h0 = 0xC1059ED8,
h1 = 0x367CD507,
h2 = 0x3070DD17,
h3 = 0xF70E5939,
h4 = 0xFFC00B31,
h5 = 0x68581511,
h6 = 0x64F98FA7,
h7 = 0xBEFA4FA4;

const int word_count =16;
const int total_count = 64;

const unsigned int k[] =
{0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
 0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
 0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
 0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
 0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
 0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
 0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
 0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};


void get_word_hash(unsigned int* w){
    unsigned int s0,s1;
    unsigned int a,b,c,d,e,f,g,h;
    for(int i=word_count;i<total_count;++i){
        s0 = rotr(w[i-15],7) ^ rotr(w[i-15],18) ^ shr(w[i-15],3);
        s1 = rotr(w[i-2],17) ^ rotr(w[i-2],19) ^ shr(w[i-2],10);
        w[i] = w[i-16] + s0 + w[i-7] + s1;
    }
    a = h0;b = h1;c = h2;d = h3;
    e = h4;f = h5;g = h6;h = h7;
    unsigned int sig0,ma,sig1,ch,t1,t2;
    for(int i=0;i<total_count;++i){
        sig0 = rotr(a,2) ^ rotr(a,13) ^ rotr(a, 22);
        ma = (a & b) ^ (a & c) ^ (b & c);
        t2 = sig0 + ma;
        sig1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
        ch = (e & f) ^ (~e & g);
        t1 = h + sig1 + ch + k[i] + w[i];
        h = g;g = f;f = e;e = d + t1;
        d = c;c = b;b = a;a = t1 + t2;

       // cout <<std::dec<< a << " " << b << " "<<c << " " <<d << " "<<e <<f << " " <<g << " "<<h<<"\n";

    }
    h0 = h0 + a;
    h1 = h1 + b;
    h2 = h2 + c;
    h3 = h3 + d;
    h4 = h4 + e;
    h5 = h5 + f;
    h6 = h6 + g;
    h7 = h7 + h;
}

void print_arr(unsigned int* arr ,int size/* = 1*/, bool hex/* = false*/){
    if( hex ){
        cout << std::hex;
        for(int i=0;i<size;++i){
            cout << (arr[i]) << " " ;
        }
        cout << "\n";
        return;
    }
    for(int i=0;i<size;++i){
        cout << asbits(arr[i]) ;
    }
    cout << "\n";
}

string print_res(bool hex/*=false*/){
    stringstream result;
    if(hex){
        //result<< std::hex<< h0 << h1 << h2 << h3 << h4 << h5 << h6;

        result << std::hex << std::setfill('0') << std::setw(2)
             << h0 << ""<< h1 << ""<< h2 << ""<< h3 << ""
             << h4 << ""<< h5 << ""<< h6 << ""
                //  << h7  - for 256 only
             << "\n";
        return result.str();
    }
    result << asbits(h0) << " "
           << asbits(h1) << " "
           << asbits(h2) << " "
           << asbits(h3) << " "
           << asbits(h4) << " "
           << asbits(h5) << " "
           << asbits(h6) << " "
              // << asbits(h7) for 256 only
           << "\n";
    return result.str();
}

string SHA2_main(string s){
    int coded_size = (s.length() / 4 )  + 1;
    unsigned int arr[(coded_size/16 + 1)*16] = {0};
    for(int i=0;i<(int)s.length();++i){
        if(i%4==0)
            arr[i/4] = 0;
        arr[i/4] |= (unsigned int)(s[i] << (8*sizeof(char)*(3 - i%4)));
    }

    int real_size  = (coded_size/16 + 1)*16;
    // cout << "Input bits = " <<std::dec << coded_size << ", res bits = "<< 32*real_size << "\n";
    arr[s.length()/4] |= (1<< (7 + 8*(3-s.length()%4)));
    arr[real_size-1] = (8*s.length()) ;
    arr[real_size-2] = (8*(long long)s.length()) >> 32;

    //cout << arr[real_size-1] <<  " " << arr[real_size-2]<<"\n";

    // print_arr(arr,real_size,true);
    unsigned int tmp[64];
    for(int i=0;i<real_size;i+=16){
        memcpy(tmp,arr,16*sizeof(int));
       // print_arr(tmp,16,true);
        SHA2::get_word_hash(tmp);
       // cout << SHA2::print_res(true) << "\n";

    }
    return SHA2::print_res(true);
}


}

