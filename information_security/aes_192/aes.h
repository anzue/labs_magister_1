#ifndef AES_H
#define AES_H

#include "operations.h"

namespace AES {


    AES_CODE* as_aes(byte* key, bool reverse = true);

    // operations on 16b codes
    void AES_block(AES_CODE* input, byte* key);
    void AES_decrypt_block(AES_CODE *input, byte * key);

    // ECB (non-paralell)
    void AES_base(byte *input,int input_size, byte * cipher);
    void AES_decrypt_base(byte *input,int input_size, byte * cipher);

    //CBC
    void AES_CBC(byte *input,int input_size, byte * key, byte* iv);
    void AES_CBC_decrypt(byte *input,int input_size, byte * key, byte* iv);

    //CFB
    void AES_CFB(byte *input,int input_size, byte * key, byte* iv);
    void AES_CFB_decrypt(byte *input,int input_size, byte * key, byte* iv);

}
#endif // AES_H
