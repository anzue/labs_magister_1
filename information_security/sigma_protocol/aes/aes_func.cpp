#include <iomanip>
#include <iostream>
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "aes.h"
#include <sstream>
using namespace std;

int char_val(char c){
    if(c>='0' && c<='9'){
        return c-'0';
    }
    return c-'a' + 10;
}

char get_coded_char(int i){
    if(i <= 9){
        return char(i + '0');
    }
    return char(i - 10 + 'a');
}


byte* from_string(string input){
    byte* res = new byte[input.length()/2];
    for(int i=0;i<input.length();i+=2){
        res[i/2] = char_val(input[i])*16 + char_val(input[i+1]);
    }
    return res;
}

string to_string(byte* input,int len){
    string res = "";
    for(int i=0;i<len;++i){
        res += get_coded_char(input[i] / 16);
        res += get_coded_char(input[i] % 16);
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
    AES::AES_block(AES::as_aes(input),key);
}


void test_2(){
      int len = string("00112233445566778899aabbccddeeff").length()/2;
      byte* input = from_string("00112233445566778899aabbccddeeff");
      byte* key = from_string("000102030405060708090a0b0c0d0e0f1011121314151617");
      cout << "Source     "; for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";

      AES::AES_base(input,16,key);

      Operations::print(input,len);
      cout << "Encrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";


      AES::AES_decrypt_base(input,16,key);

      Operations::print(input,len);
      cout << "Decrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";
}



string AES_enc(string input_str,string key_str ,string iv_str){
    int len = input_str.length();
    byte* input = from_string(input_str);
    byte* key = from_string(key_str);
    byte* iv = from_string(iv_str);
    stringstream sstream;
    cout << "\n" << input_str;
    cout << "\nSource     "; for(int i=0;i<len/2;++i)cout << std::hex << (int)input[i];cout <<"\n";
    //for(int i=0;i<len/2;++i)sstream << std::hex << (int)input[i];cout <<"\n";
    AES::AES_base(input,len/2,key);
    for(int i=0;i<len/2;++i){
        sstream << std::setfill('0') << std::setw(2) << std::hex << (int)input[i];
     //   cout << std::dec << i << " " <<sstream.str().length() << "\n";
    }//cout << std::endl;

    Operations::print(input,input_str.length());
  //  string res = to_string(input,len/8);
  //  cout << (void*)input <<std::endl;
    delete []input;
    delete []key;
    delete []iv;
    return sstream.str();
}

string AES_dec(string input_str,string key_str,string iv_str){
    int len = input_str.length();
    byte* input = from_string(input_str);
    byte* key = from_string(key_str);
    byte* iv = from_string(iv_str);

    AES::AES_decrypt_base(input,len/2,key);

    Operations::print(input,input_str.length()/2);
    return to_string(input,len/2);
}


string CBC_enc(string input_str,string key_str/* = "000102030405060708090a0b0c0d0e0f1011121314151617"*/,string iv_str/* = "32143546589743fdaf45daf5eafafaa0"*/){
      int len = input_str.length();
      byte* input = from_string(input_str);
      byte* key = from_string(key_str);
      byte* iv = from_string(iv_str);
      cout << "\nSource     "; for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";

      AES::AES_CBC(input,len,key,iv);

      Operations::print(input,input_str.length());
      cout << "Encrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";


      return to_string(input,len/4);

}

string CBC_dec(string input_str,string key_str/* = "000102030405060708090a0b0c0d0e0f1011121314151617"*/,string iv_str/* = "32143546589743fdaf45daf5eafafaa0"*/){
    int len = input_str.length();
    byte* input = from_string(input_str);
    byte* key = from_string(key_str);
    byte* iv = from_string(iv_str);

    AES::AES_CBC_decrypt(input,len,key,iv);

    Operations::print(input,input_str.length());
    cout << "Decrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";
    return to_string(input,len/4);
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

      AES::AES_CFB_decrypt(input,32,key,iv);

      Operations::print(input,len);
      cout << "Decrypted  ";for(int i=0;i<len;++i)cout << std::hex << (int)input[i];cout <<"\n";
}

std::ifstream::pos_type filesize(string filename){
    std::ifstream in(filename.c_str(), std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}


void encrypt_file_CBC(string file_in, string file_out){
    int start_time = clock();
    int size = 256*256;
    int last_block_size = filesize(file_in)%size;
    ifstream myfile;
    myfile.open(file_in.c_str(),ios::binary);

    byte buffer[size];

    byte* key = from_string("000102030405060708090a0b0c0d0e0f1011121314151617");
    byte* iv =  from_string("00112233445566778899aabbccddeeff");
    ofstream outfile;

    outfile.open(file_out.c_str(),ios::binary  | ios::out);

    while(myfile.read((char*)buffer,size)){
        AES::AES_CBC(buffer,size,key,iv);
        outfile.write((char*)buffer,size);
    }
    if(last_block_size){
        memset(buffer+last_block_size, 0 ,size - last_block_size);
        AES::AES_CBC(buffer,size,key,iv);
        outfile.write((char*)buffer,size);
    }
    myfile.close();
    outfile.close();
    cout << "Time = " << 1.*(clock() - start_time) / CLOCKS_PER_SEC << std::endl;

}

void decrypt_file_CBC(string file_in, string file_out){
    int start_time = clock();
    int size = 256*256;
    int last_block_size = filesize(file_in)%size;

    ifstream myfile;
    myfile.open(file_in.c_str(),ios::binary);

    byte buffer[size];

    byte* key = from_string("000102030405060708090a0b0c0d0e0f1011121314151617");
    byte* iv =  from_string("00112233445566778899aabbccddeeff");
    ofstream outfile;
    outfile.open(file_out.c_str(),ios::binary  | ios::out);

    while(myfile.read((char*)buffer,size)){
        AES::AES_CBC_decrypt(buffer,size,key,iv);
        outfile.write((char*)buffer,size);
    }
    if(myfile.tellg()%size){
        memset(buffer+last_block_size, 0,size -last_block_size);
        AES::AES_CBC_decrypt(buffer,size,key,iv);
        outfile.write((char*)buffer,size);
    }
    myfile.close();
    outfile.close();

    cout << "Time = " << 1.*(clock() - start_time) / CLOCKS_PER_SEC << std::endl;
}
