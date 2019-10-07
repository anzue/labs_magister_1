#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include "sha_224.cpp"

using namespace std;

void test_rotr()
{
    cout << asbits(SHA2::h0) << "\n" << asbits(rotr(SHA2::h0,5))  << endl;
}

void test_sha(){
    string code = "The quick brown fox jumps over the lazy cog";

    SHA2::print_res(true);

    SHA2::SHA_main(code);

    /*
    SHA-256("The quick brown fox jumps over the lazy dog")
    = D7A8FBB3 07D78094 69CA9ABC B0082E4F 8D5651E4 6D3CDB76 2D02D0BF 37C9E592

    SHA-256("The quick brown fox jumps over the lazy cog")
    = E4C4D8F3 BF76B692 DE791A17 3E053211 50F7A345 B46484FE 427F6ACC 7ECC81BE
*/

}



int main(){
    test_sha();
}
