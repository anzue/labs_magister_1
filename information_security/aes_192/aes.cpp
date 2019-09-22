#include <assert.h>
#include "aes.h"


namespace AES
{
    AES_CODE* as_aes(byte* key, bool reverse = true){
        int (*res)[ARR_SIZE] = new int[ARR_SIZE][ARR_SIZE];
        for(int i=0;i<ARR_SIZE;++i){
            for(int j=0;j<ARR_SIZE;++j){
                if ( reverse)
                    res[j][i] = key[i*ARR_SIZE+j];
                else
                    res[i][j] = key[i*ARR_SIZE+j];
            }
        }
        return (AES_CODE*)res;
        //   return (AES_CODE*)(key);
    }

    byte* as_byte_array(AES_CODE* code){
        byte* arr = new byte[ARR_SIZE*ARR_SIZE];
        for(int i=0;i<ARR_SIZE;++i){
            for(int j=0;j<ARR_SIZE;++j){
                arr[i*ARR_SIZE+j] = *code[i][j];
            }
        }
        return arr;
       // return (byte*)code;
    }



    void AES_block(AES_CODE* input, byte* key){
        byte expanded[ ARR_SIZE*ARR_SIZE* (Nr+1) ];

        Operations::print(key,16,4,3);

        Operations::KeyExpansion(expanded,key);

        Operations::print(expanded,ARR_SIZE*ARR_SIZE* (Nr+1) , 4,3 );
        out ("Starting\n");
        Operations::PrintState(input);
        AES_CODE* round_key = as_aes(expanded);

        Operations::AddRoundKey(input,round_key);
        out("AddRoundKey\n");
        Operations::PrintState(input);

        for(int i=1;i<Nr;++i){
            delete round_key;
            round_key = as_aes(expanded + i* ARR_SIZE*ARR_SIZE);
            out("Starting round "); out(std::dec);out(i);out(std::hex);out("\n");
            Operations::PrintState(input);
            out ("Key\n");
            Operations::PrintState(round_key);

            Operations::SubBytes(input);
            out ("SubBytes\n");
            Operations::PrintState(input);

            Operations::ShiftRows(input);
            out ("ShiftRows\n");
            Operations::PrintState(input);

            Operations::MixColumns(input);
            out ("MixColumns\n");
            Operations::PrintState(input);

            Operations::AddRoundKey(input,round_key);
            out ("AddRoundKey\n");
            Operations::PrintState(input);
        }

        out ("Last iter\n");

        Operations::SubBytes(input);
        out ("SubBytes\n");
        Operations::PrintState(input);

        Operations::ShiftRows(input);
        out( "ShiftRows\n");
        Operations::PrintState(input);

        round_key = as_aes(expanded + (Nr)* ARR_SIZE*ARR_SIZE);
        Operations::AddRoundKey(input,round_key);
        out( "AddRoundKey\n");
        Operations::PrintState(input);
    }

    // Both input and key 16 bytes each
    void AES_block(byte* input,byte* key){

        AES_CODE* input_as_aes = as_aes(input);

        AES_block(input_as_aes,key);

        for(int i=0;i<ARR_SIZE;++i){
            for(int j=0;j<ARR_SIZE;++j){
                input[i*ARR_SIZE+j] = *input_as_aes[i][j];
            }
        }

        delete input_as_aes;
    }


    void AES_base(byte *input,int input_size, byte * cipher){
        assert(input_size%16 == 0);
        //AES_CODE tmp;
        for(int i=0;i<input_size; i+=16){
            /*
            memcpy(tmp,input+i,16);
            AES_block(&tmp, cipher);
            memcpy(input+i,tmp,16);*/
            AES_block(input+i,cipher);
        }

    }

    void AES_decrypt_base(byte *input,int input_size, byte * cipher){
        assert(input_size%16 == 0);
        for(int i=0;i<input_size; i+=16){
            AES_decrypt_block(input+i,cipher);
        }
    }

    // Both input and key 16 bytes each
    void AES_decrypt_block(byte* input,byte* key){

        AES_CODE* input_as_aes = as_aes(input,false);

        AES_decrypt_block(input_as_aes,key);

        for(int i=0;i<ARR_SIZE;++i){
            for(int j=0;j<ARR_SIZE;++j){
                input[i*ARR_SIZE+j] = *input_as_aes[j][i];
            }
        }
        delete input_as_aes;
    }
    void AES_decrypt_block(AES_CODE *input, byte * key){
        byte expanded[ ARR_SIZE*ARR_SIZE* (Nr+1) ];

        //Operations::print(key,16,4,3);

        Operations::KeyExpansion(expanded,key);

        out("Expanded\n");
        Operations::print(expanded,ARR_SIZE*ARR_SIZE* (Nr+1) , 4,3 );

        Operations::print(key,4*6, 4,3 );

        out("Starting\n");
        Operations::PrintState(input);
        AES_CODE* round_key = as_aes(expanded + ARR_SIZE*ARR_SIZE* Nr);
        out("Round key\n");
        Operations::PrintState(round_key);

        Operations::AddRoundKey(input,round_key);
        out("AddRoundKey\n");
        Operations::PrintState(input);

        for(int i=Nr-1;i>=1;--i){
            delete round_key;
            round_key = as_aes(expanded + i* ARR_SIZE*ARR_SIZE);

            out("Starting round "); out(std::dec);out(i);out(std::hex);out("\n");
            Operations::PrintState(input);
            out("Key\n");
            Operations::PrintState(round_key);

            Operations::InvShiftRows(input);
            out("ShiftRows\n");
            Operations::PrintState(input);

            Operations::InvSubBytes(input);
            out("SubBytes\n");
            Operations::PrintState(input);

            Operations::AddRoundKey(input,round_key);
            out("AddRoundKey\n");
            Operations::PrintState(input);

            Operations::InvMixColumns(input);
            out("MixColumns\n");
            Operations::PrintState(input);


        }

        out("Last iter\n");

        Operations::InvShiftRows(input);
        out("ShiftRows\n");
        Operations::PrintState(input);

        Operations::InvSubBytes(input);
        out("SubBytes\n");
        Operations::PrintState(input);

        round_key = as_aes(expanded + 0);
        Operations::AddRoundKey(input,round_key);
        out("AddRoundKey\n");
        Operations::PrintState(input);
        delete round_key;
    }
};
