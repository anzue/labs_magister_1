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
        cout << "Starting\n";
        Operations::PrintState(input);
        AES_CODE* round_key = as_aes(expanded);

        Operations::AddRoundKey(input,round_key);
        cout << "AddRoundKey\n";
        Operations::PrintState(input);

        for(int i=1;i<=Nr;++i){
            delete key;
            round_key = as_aes(expanded + i* ARR_SIZE*ARR_SIZE);

            cout << "Starting round " << std::dec<< i << std::hex << "\n";
            Operations::PrintState(input);
            cout << "Key\n";
            Operations::PrintState(round_key);

            Operations::SubBytes(input);
            cout << "SubBytes\n";
            Operations::PrintState(input);

            Operations::ShiftRows(input);
            cout << "ShiftRows\n";
            Operations::PrintState(input);

            Operations::MixColumns(input);
            cout << "MixColumns\n";
            Operations::PrintState(input);

            Operations::AddRoundKey(input,round_key);
            cout << "AddRoundKey\n";
            Operations::PrintState(input);
        }
    }

    // Both input and key 16 bytes each
    void AES_block(byte* input,byte* key){

        AES_CODE* input_as_aes = as_aes(input);

        AES_block(input_as_aes,key);

        delete input_as_aes;
    }


    void AES_base(byte *input,int input_size, byte * cipher){
        assert(input_size%16 == 0);

        for(int i=0;i<input_size; i+=16){

        }

    }
};
