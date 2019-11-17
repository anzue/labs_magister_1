#ifndef AES_FUNC_H
#define AES_FUNC_H

#include<string>
using std::string;

string CBC_enc(string input_str,string key_str = "000102030405060708090a0b0c0d0e0f1011121314151617",string iv_str = "32143546589743fdaf45daf5eafafaa0");
string CBC_dec(string input_str,string key_str = "000102030405060708090a0b0c0d0e0f1011121314151617",string iv_str = "32143546589743fdaf45daf5eafafaa0");


string AES_enc(string input_str,string key_str = "000102030405060708090a0b0c0d0e0f1011121314151617",string iv_str = "32143546589743fdaf45daf5eafafaa0");
string AES_dec(string input_str,string key_str = "000102030405060708090a0b0c0d0e0f1011121314151617",string iv_str = "32143546589743fdaf45daf5eafafaa0");

#endif // AES_FUNC_H
