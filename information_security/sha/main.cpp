#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include "sha_224.cpp"
#include "sha_3_512.cpp"

void test_rotr()
{
    cout << asbits(SHA2::h0) << "\n" << asbits(rotr(SHA2::h0,5))  << endl;
}

void test_sha(){


/*
SHA-224("The quick brown fox jumps over the lazy dog")
 = 730E109B D7A8A32B 1CB9D9A0 9AA2325D 2430587D DBC0C38B AD911525
*/
    string code = "The quick brown fox jumps over the lazy dog";
    SHA2::SHA2_main(code);

    // old for 256 version
    /*
    SHA-256("The quick brown fox jumps over the lazy dog")
    = D7A8FBB3 07D78094 69CA9ABC B0082E4F 8D5651E4 6D3CDB76 2D02D0BF 37C9E592

    SHA-256("The quick brown fox jumps over the lazy cog")
    = E4C4D8F3 BF76B692 DE791A17 3E053211 50F7A345 B46484FE 427F6ACC 7ECC81BE
*/

}

void test_sha3(){
   // string code = "The quick brown fox jumps over the lazy dog";

    /*
        SHA3-512("")
        a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26
    */

   //string code = "The quick brown fox jumps over the lazy dog";
    string code = "";
    SHA3::SHA3_main(code);

}


void test_composite(){
    char cipher1[] = {45,56,73,42,56,57,58,57,61,0};
    char cipher2[] = {45,56,73,42,56,57,58,57,61,0};

    string code = "The quick brown fox jumps over the lazy dog";

    cout << string(cipher1) + code << "\n";
    SHA2::SHA2_main(string(cipher1) + code);
    string s;
    getline(result,s);
    cout << string(cipher2) + s << "\n";
    SHA3::SHA3_main(string(cipher2) + s);
}




int main(){
    test_sha3();
}
