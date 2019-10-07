#include "defines.h"

#define word_size 64
#define rotl(x,a) ((x<<a) | (x >> (word_size - a)) )
#define asbits(x) (std::bitset<8*sizeof(word)>(x))

//#define swap(x)   \
//    (x >> 56) |\
//    ((x >> 40) & 0x000000000000FF00ULL) |\
//    ((x >> 24) & 0x0000000000FF0000ULL) |\
//    ((x >>  8) & 0x00000000FF000000ULL) |\
//    ((x <<  8) & 0x000000FF00000000ULL) |\
//    ((x << 24) & 0x0000FF0000000000ULL) |\
//    ((x << 40) & 0x00FF000000000000ULL) |\
//     (x << 56)
typedef unsigned long long word;

using namespace std;

namespace SHA3 {
    const unsigned int rounds = 24;
    const unsigned int block_size_bits = 72*8;
    const unsigned int block_size = 72;
    const unsigned int hash_len = 512/64;

    const word mask[rounds] =
      {
        0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
        0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
        0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
        0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
        0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
        0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
        0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
        0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
      };

    unsigned char buffer[block_size];
    word hash[block_size] = {0};

    void get_word_hash(word* data)
    {
      for (unsigned int i = 0; i < block_size/8; i++)
        hash[i] ^= data[i];


      word c[5];
      word tmp1,tmp2;
      for (unsigned int round = 0; round < rounds; round++)
      {

        for (unsigned int i = 0; i < 5; i++){
          c[i] = hash[i] ^ hash[i + 5] ^ hash[i + 10] ^ hash[i + 15] ^ hash[i + 20];
        //  cout << std::dec << coefficients[i] << " ";
        }
      //  for(int i=0;i<1;++i)cout<<hash[i] << " ";cout<<"\n";cout<<"\n";
        for (unsigned int i = 0; i < 5; i++)
        {
          tmp1 = c[(i + 4)%5] ^ rotl(c[(i + 1)%5], 1);
          hash[i     ] ^= tmp1;
          hash[i +  5] ^= tmp1;
          hash[i + 10] ^= tmp1;
          hash[i + 15] ^= tmp1;
          hash[i + 20] ^= tmp1;
        }

       // for(int i=0;i<1;++i)cout<<hash[i] << " ";cout<<"\n";

        tmp2 = hash[1];
        tmp1 = hash[10]; hash[10] = rotl(tmp2,  1); tmp2 = tmp1;tmp1 = hash[ 7]; hash[ 7] = rotl(tmp2,  3); tmp2 = tmp1;tmp1 = hash[11]; hash[11] = rotl(tmp2,  6); tmp2 = tmp1;
        tmp1 = hash[17]; hash[17] = rotl(tmp2, 10); tmp2 = tmp1;tmp1 = hash[18]; hash[18] = rotl(tmp2, 15); tmp2 = tmp1;tmp1 = hash[ 3]; hash[ 3] = rotl(tmp2, 21); tmp2 = tmp1;
        tmp1 = hash[ 5]; hash[ 5] = rotl(tmp2, 28); tmp2 = tmp1;tmp1 = hash[16]; hash[16] = rotl(tmp2, 36); tmp2 = tmp1;tmp1 = hash[ 8]; hash[ 8] = rotl(tmp2, 45); tmp2 = tmp1;
        tmp1 = hash[21]; hash[21] = rotl(tmp2, 55); tmp2 = tmp1;tmp1 = hash[24]; hash[24] = rotl(tmp2,  2); tmp2 = tmp1;tmp1 = hash[ 4]; hash[ 4] = rotl(tmp2, 14); tmp2 = tmp1;
        tmp1 = hash[15]; hash[15] = rotl(tmp2, 27); tmp2 = tmp1;tmp1 = hash[23]; hash[23] = rotl(tmp2, 41); tmp2 = tmp1;tmp1 = hash[19]; hash[19] = rotl(tmp2, 56); tmp2 = tmp1;
        tmp1 = hash[13]; hash[13] = rotl(tmp2,  8); tmp2 = tmp1;tmp1 = hash[12]; hash[12] = rotl(tmp2, 25); tmp2 = tmp1;tmp1 = hash[ 2]; hash[ 2] = rotl(tmp2, 43); tmp2 = tmp1;
        tmp1 = hash[20]; hash[20] = rotl(tmp2, 62); tmp2 = tmp1;tmp1 = hash[14]; hash[14] = rotl(tmp2, 18); tmp2 = tmp1;tmp1 = hash[22]; hash[22] = rotl(tmp2, 39); tmp2 = tmp1;
        tmp1 = hash[ 9]; hash[ 9] = rotl(tmp2, 61); tmp2 = tmp1;tmp1 = hash[ 6]; hash[ 6] = rotl(tmp2, 20); tmp2 = tmp1;hash[1] = rotl(tmp2, 44);


       // for(int i=0;i<1;++i)cout<<hash[i] << " ";cout<<"\n";


        for ( int j = 0; j < 25; j += 5)
        {
          word tmp1 = hash[j];
          word tmp2 = hash[j + 1];
          hash[j]     ^= hash[j + 2] & ~tmp2;
          hash[j + 1] ^= hash[j + 3] & ~hash[j + 2];
          hash[j + 2] ^= hash[j + 4] & ~hash[j + 3];
          hash[j + 3] ^= tmp1 & ~hash[j + 4];
          hash[j + 4] ^= tmp2 & ~tmp1;
        }
        hash[0] ^= mask[round];

       // for(int i=0;i<5;++i)cout<<hash[i] << " ";cout<<"\n";

      }
    }

    void print_arr(word* arr ,int size = 1, bool hex = false){
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

    void print_res(bool hex=false){
        if(hex){
            cout << std::hex;
            for(int i=0;i<hash_len;++i){
                for(int j=0;j<8;++j){
                    cout << ((hash[i] >> (8*j))&(0xff));
                }
                //cout<< " ";
            }
            cout<<"\n";
            return;
        }

    }

    void SHA3_main(string s){
        int coded_size = (s.length() / 8 )  + 1;
        word arr[(coded_size/(block_size) + 1)*(block_size)] = {0};

        unsigned char* arr_as_byte = (unsigned char*)arr;

        for(int i=0;i<(int)s.length();++i){
            arr_as_byte[i] = s[i] ;
        }

        int real_size  = (coded_size/(block_size) + 1)*(block_size);
        arr_as_byte[s.length()] = 0x06;
        arr_as_byte[real_size-1] = 0x80;

        word tmp[block_size];
        for(int i=0;i<real_size;i+=block_size){
            memcpy(tmp,arr,block_size);
          //  print_arr(tmp,16,false);
            get_word_hash(tmp);
         //   print_res(true);

        }
        print_res(true);
    }


}
