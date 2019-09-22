#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <iostream>
using namespace std;

typedef unsigned char byte;

const static int ARR_SIZE = 4;
typedef unsigned char AES_CODE[4][4];

const static int Nk = 6;
const static int Nr = 12;


namespace Operations {

    void print(byte* word, int count , int mod = 1e9 , int val = 1e9);

    void PrintState(AES_CODE* input);

    void SubBytes(AES_CODE* input);

    void ShiftRows(AES_CODE* input);

    void MixColumns(AES_CODE* input);

    void AddRoundKey(AES_CODE* input, AES_CODE* round_key);

    void KeyExpansion(byte* all_key,byte* key);

};

#endif // OPERATIONS_H
