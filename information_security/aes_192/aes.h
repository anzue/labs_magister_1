#ifndef AES_H
#define AES_H

#include "operations.h"


namespace AES {

    // Both input and key 16 bytes each
    void AES_block(byte* input,byte* key);

    void AES_base(byte *input,int input_size, byte * cipher, int cipher_size);
}
#endif // AES_H
