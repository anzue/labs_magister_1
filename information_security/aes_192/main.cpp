#include <iostream>
#include "aes.h"

using namespace std;

int char_val(char c){
    if(c>='0' && c<='9'){
        return c-'0';
    }
    return c-'a' + 10;
}

byte* from_string(string input){
    byte* res = new byte[input.length()/2];
    for(int i=0;i<input.length();i+=2){
        res[i/2] = char_val(input[i])*16 + char_val(input[i+1]);
    }
    return res;
}


void test_cipher_doc(){
  //  byte cipher[] = {0x8e ,0x73 ,0xb0 ,0xf7 ,0xda ,0x0e ,0x64 ,0x52 ,0xc8 ,0x10 ,0xf3 ,0x2b ,
  //                   0x80 ,0x90 ,0x79 ,0xe5 ,0x62 ,0xf8 ,0xea ,0xd2 ,0x52 ,0x2c ,0x6b ,0x7b };
   byte* cipher = from_string("000102030405060708090a0b0c0d0e0f1011121314151617");
   byte expanded[ ARR_SIZE*ARR_SIZE* (Nr+1) ];
   Operations::KeyExpansion(expanded, cipher);

   for(int i=0;i<ARR_SIZE*ARR_SIZE* (Nr+1);++i){
       out(std::hex);out((int) expanded[i]);out((i%4 == 3?"\n" :" "));
   }
}

void test_1_doc(){
    byte key[]   = {0x2b ,0x7e ,0x15 ,0x16 ,0x28 ,0xae ,0xd2 ,0xa6 ,0xab ,0xf7 ,0x15 ,0x88 ,0x09 ,0xcf ,0x4f ,0x3c};
    byte input[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    AES::AES_block(input,key);
}


void test_2(){
      int len = string("00112233445566778899aabbccddeeff").length()/2;
      byte* input = from_string("00112233445566778899aabbccddeeff");
      byte* key = from_string("000102030405060708090a0b0c0d0e0f1011121314151617");
      cout << "Source     "; for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";

      AES::AES_base(input,16,key);

      Operations::print(input,string("00112233445566778899aabbccddeeff").length());
      cout << "Encrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";

      for(int i=0;i<6*4;++i)
          cout << (int)key[i] << " ";

      cout<<std::endl;

      AES::AES_decrypt_base(input,16,key);

      Operations::print(input,string("00112233445566778899aabbccddeeff").length());
      cout << "Decrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";
}


void test_CBC(){
      int len = string("00112233445566778899aabbccddeeff").length()/2 * 2;
      byte* input = from_string("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff");
      byte* key = from_string("000102030405060708090a0b0c0d0e0f1011121314151617");
     // byte* iv = from_string("00112233445566778899aabbccddeeff");
      byte* iv = from_string  ("32143546589743fdaf45daf5eafafaa0");
      cout << "Source     "; for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";

      AES::AES_CBC(input,32,key,iv);

      Operations::print(input,string("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff").length());
      cout << "Encrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";

      for(int i=0;i<6*4;++i)
          cout << (int)key[i] << " ";

      cout<<std::endl;

      AES::AES_CBC_decrypt(input,32,key,iv);

      Operations::print(input,string("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff").length());
      cout << "Decrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";
}



void test_CFB(){
      int len = string("00112233445566778899aabbccddeeff").length()/2 * 2;
      byte* input = from_string("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff");
      byte* key = from_string("000102030405060708090a0b0c0d0e0f1011121314151617");
      byte* iv = from_string("00112233445566778899aabbccddeeff");
    //  byte* iv = from_string  ("32143546589743fdaf45daf5eafafaa0");
      cout << "Source     "; for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";

      AES::AES_CFB(input,32,key,iv);

      Operations::print(input,len);
      cout << "Encrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";

      for(int i=0;i<6*4;++i)
          cout << (int)key[i] << " ";

      cout<<std::endl;

      AES::AES_CFB_decrypt(input,32,key,iv);

      Operations::print(input,len);
      cout << "Decrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";
}



int main()
{
    test_CFB();
    return 0;
}
