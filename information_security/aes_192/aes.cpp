#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "aes.h"


namespace AES
{
    AES_CODE* as_aes(byte* key, bool reverse){
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
    }

    void AES_block(AES_CODE* input, byte* key){
        Operations::print(key,ARR_SIZE*ARR_SIZE* (Nr+1) , 4,3 );
        out ("Starting\n");
        Operations::PrintState(input);

        out("Key\n");
        Operations::print(key,16,4,3);

        Operations::AddRoundKey(input,key + 0*ARR_SIZE*ARR_SIZE);
        out("AddRoundKey\n");
        Operations::PrintState(input);

        for(int i=1;i<Nr;++i){
            out("Starting round "); out(std::dec);out(i);out(std::hex);out("\n");
            Operations::PrintState(input);


            Operations::SubBytes(input);
            out ("SubBytes\n");
            Operations::PrintState(input);

            Operations::ShiftRows(input);
            out ("ShiftRows\n");
            Operations::PrintState(input);

            Operations::MixColumns(input);
            out ("MixColumns\n");
            Operations::PrintState(input);

            out ("Key\n");
            Operations::print(key + i*ARR_SIZE*ARR_SIZE,16);

            Operations::AddRoundKey(input,key + i*ARR_SIZE*ARR_SIZE);
            out ("AddRoundKey\n");out(std::endl);
            Operations::PrintState(input);
        }

        out ("Last iter\n");

        Operations::SubBytes(input);
        out ("SubBytes\n");
        Operations::PrintState(input);

        Operations::ShiftRows(input);
        out( "ShiftRows\n");
        Operations::PrintState(input);

        out ("Key\n");
        Operations::print(key + Nr*ARR_SIZE*ARR_SIZE,16);

        Operations::AddRoundKey(input,key + Nr*ARR_SIZE*ARR_SIZE);
        out( "AddRoundKey\n");
        Operations::PrintState(input);
    }


    inline void init_from_byte_array(AES_CODE* code,byte* input){
        for(int i=0;i<ARR_SIZE;++i){
            for(int j=0;j<ARR_SIZE;++j){
                *code[i][j] = input[j*ARR_SIZE + i];
            }
        }
    }

    inline void copy_to_byte_array(AES_CODE* code,byte* input){
        for(int i=0;i<ARR_SIZE;++i){
            for(int j=0;j<ARR_SIZE;++j){
                 input[j*ARR_SIZE + i] = *code[i][j];
            }
        }
    }

    void AES_decrypt_block(AES_CODE *input, byte * key){

        out("Key\n");
        Operations::print(key,ARR_SIZE*ARR_SIZE* (Nr+1) , 4,3 );

        Operations::print(key,4*6, 4,3 );

        out("Starting\n");
        Operations::PrintState(input);
        out("Round key\n");
        Operations::print(key, 16,4,3);

        Operations::AddRoundKey(input,key + Nr*ARR_SIZE*ARR_SIZE);
        out("AddRoundKey\n");
        Operations::PrintState(input);

        for(int i=Nr-1;i>=1;--i){
            out("Starting round "); out(std::dec);out(i);out(std::hex);out("\n");
            Operations::PrintState(input);
            Operations::InvShiftRows(input);
            out("ShiftRows\n");
            Operations::PrintState(input);

            Operations::InvSubBytes(input);
            out("SubBytes\n");
            Operations::PrintState(input);

            out("Key\n");
            Operations::print(key + i*ARR_SIZE*ARR_SIZE,16,4,3);

            Operations::AddRoundKey(input,key + i*ARR_SIZE*ARR_SIZE);
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

        Operations::AddRoundKey(input,key + 0*ARR_SIZE*ARR_SIZE);
        out("AddRoundKey\n");
        Operations::PrintState(input);
    }


    void AES_base(byte *input,int input_size, byte * key){

        assert(input_size%16 == 0);
        int (*tmp)[ARR_SIZE] = new int[ARR_SIZE][ARR_SIZE];
        byte expanded[ ARR_SIZE*ARR_SIZE* (Nr+1) ];
        Operations::KeyExpansion(expanded,key);
        AES_CODE* code = (AES_CODE*)tmp;

        for(int i=0;i<input_size; i+=16){
            init_from_byte_array(code,input+i);
            AES_block(code,expanded);
            copy_to_byte_array(code,input+i);
        }
        //delete tmp;
    }

    void AES_decrypt_base(byte *input,int input_size, byte * key){
        assert(input_size%16 == 0);
        int (*tmp)[ARR_SIZE] = new int[ARR_SIZE][ARR_SIZE];
        byte expanded[ ARR_SIZE*ARR_SIZE* (Nr+1) ];
        Operations::KeyExpansion(expanded,key);
        AES_CODE* code = (AES_CODE*)tmp;

        for(int i=0;i<input_size; i+=16){
            init_from_byte_array(code,input+i);
            AES_decrypt_block(code,expanded);
            copy_to_byte_array(code,input+i);
        }
        //delete tmp;
    }

    void AES_CBC(byte *input,int input_size, byte * key, byte* iv){
        assert(input_size%16 == 0);
        int (*tmp)[ARR_SIZE] = new int[ARR_SIZE][ARR_SIZE];
        AES_CODE* code = (AES_CODE*)tmp;

        byte *cur_xor = new byte[ARR_SIZE*ARR_SIZE];
        memcpy(cur_xor,iv,ARR_SIZE*ARR_SIZE);

        byte expanded[ ARR_SIZE*ARR_SIZE* (Nr+1) ];
        Operations::KeyExpansion(expanded,key);

        for(int i=0;i<input_size; i+=16){
            for(int j=i;j<i+ARR_SIZE*ARR_SIZE;++j){
                cur_xor[j-i]^=input[j];
            }
            init_from_byte_array(code,cur_xor);
            AES_block(code,expanded);
            copy_to_byte_array(code,input+i);
            memcpy(cur_xor,input+i,ARR_SIZE*ARR_SIZE);

        }
    }

    void AES_CBC_decrypt(byte *input,int input_size, byte * key, byte* iv){
        assert(input_size%16 == 0);
        int (*tmp)[ARR_SIZE] = new int[ARR_SIZE][ARR_SIZE];
        AES_CODE* code = (AES_CODE*)tmp;

        byte *cur_xor = new byte[ARR_SIZE*ARR_SIZE];
        byte *save_xor = new byte[ARR_SIZE*ARR_SIZE];

        byte expanded[ ARR_SIZE*ARR_SIZE* (Nr+1) ];
        Operations::KeyExpansion(expanded,key);

        memcpy(cur_xor,iv,ARR_SIZE*ARR_SIZE);

        for(int i=0;i<input_size; i+=16){
            memcpy(save_xor,input+i,ARR_SIZE*ARR_SIZE);
            init_from_byte_array(code,input+i);
            AES_decrypt_block(code,expanded);

            copy_to_byte_array(code,input+i);

            for(int j=i;j<i+ARR_SIZE*ARR_SIZE;++j){
                input[j]^= cur_xor[j-i];
            }
            swap(save_xor,cur_xor);
        }
    }



    void AES_CFB(byte *input,int input_size, byte * key, byte* iv){
        assert(input_size%16 == 0);
        int (*tmp)[ARR_SIZE] = new int[ARR_SIZE][ARR_SIZE];
        AES_CODE* code = (AES_CODE*)tmp;

        byte *cur_xor = new byte[ARR_SIZE*ARR_SIZE];
        memcpy(cur_xor,iv,ARR_SIZE*ARR_SIZE);

        byte expanded[ ARR_SIZE*ARR_SIZE* (Nr+1) ];
        Operations::KeyExpansion(expanded,key);

        for(int i=0;i<input_size; i+=16){

            init_from_byte_array(code,cur_xor);
            AES_block(code,expanded);
            copy_to_byte_array(code,cur_xor);

            for(int j=i;j<i+ARR_SIZE*ARR_SIZE;++j){
                input[j]^=cur_xor[j-i];
            }
            memcpy(cur_xor,input+i,ARR_SIZE*ARR_SIZE);
        }
    }

    void AES_CFB_decrypt(byte *input,int input_size, byte * key, byte* iv){
        assert(input_size%16 == 0);
        int (*tmp)[ARR_SIZE] = new int[ARR_SIZE][ARR_SIZE];
        AES_CODE* code = (AES_CODE*)tmp;

        byte *cur_xor = new byte[ARR_SIZE*ARR_SIZE];
        byte *save_xor = new byte[ARR_SIZE*ARR_SIZE];

        memcpy(cur_xor,iv,ARR_SIZE*ARR_SIZE);

        byte expanded[ ARR_SIZE*ARR_SIZE* (Nr+1) ];
        Operations::KeyExpansion(expanded,key);

        for(int i=0;i<input_size; i+=16){
            init_from_byte_array(code,cur_xor);
            AES_block(code,expanded);
            copy_to_byte_array(code,cur_xor);

            memcpy(save_xor,input+i,ARR_SIZE*ARR_SIZE);
            for(int j=i;j<i+ARR_SIZE*ARR_SIZE;++j){
                input[j]^=cur_xor[j-i];
            }
            memcpy(cur_xor,save_xor,ARR_SIZE*ARR_SIZE);
        }
    }


};

