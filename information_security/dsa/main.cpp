#include <iostream>

#include "sha_224.h"
#include "bigint.h"

using namespace std;

int BigInt::DEBUG = 0;


BigInt get_random_big_int(int bytes){
    BigInt res = 0;
    for(int i=0;i<bytes;++i){
        res.print(cout,BigInt::PrintType::HEX,3);
      /*  BigInt(mod_part).print(cout,BigInt::PrintType::HEX);
        BigInt(rand()%mod_part).print(cout,BigInt::PrintType::HEX);

        res.print(cout,BigInt::PrintType::HEX);
        (res * BigInt(mod_part)).print(cout,BigInt::PrintType::HEX);
        (res * BigInt(mod_part) + BigInt(rand()%mod_part)).print(cout,BigInt::PrintType::HEX);
        cout<<"\n";*/
        res = res * BigInt((unsigned int)mod_part) + BigInt((unsigned int)rand()%mod_part);
    }

    res.print(cout,BigInt::PrintType::HEX);
    return res;
}



bool is_prime(BigInt val){
   // val.print();

    for(int i=0;i<10;++i){
        BigInt a = get_random_big_int(val.get_len());
        a.print();
        if(a >= val || a == 0){
            --i;continue;
        }
      /*  cout << i << "\n";
        a.print();
        binpow(a, val - one,val).print();
        val.print();cout<<"\n";*/
        if(! (binpow(a, val - one,val) == one)){
            /*
            cout << "divizor = \n";
            a.print(std::cout,BigInt::PrintType::DEC,10);
            val.print(std::cout,BigInt::PrintType::DEC,10);
            cout << "\n\n";
           */
           // (val/a).print();a.print();
            return false;
        }
    }
    return true;
}

BigInt get_random_prime(int bytes){
    //"203391339108018642348853042914050827989603460621" one of primes found for 80 bit
    BigInt q = get_random_big_int(bytes);
    if(q%two == 0)
        q += one;

    BigInt hungred = 10;

    while(!is_prime(q)){
        q += two;
       // if(q%BigInt(10) == one)
            q.print();
    }
    return q;
}


int main()
{
    srand(time(0));

    BigInt::DEBUG = 1;
    // message
    string code = "The quick brown fox jumps over the lazy dog";
    cout << "After SHA-224\n";
    string s = SHA2::SHA2_main(code);
    cout << s;
    BigInt coded = BigInt(s,16);


    int L = 1024;
    int N = 160;

    //BigInt q = get_random_prime(N/8);
    BigInt q = get_random_prime(1);
    cout << "Chosen prime :";q.print();

    cout << "Coded len = " << std::dec << 8*coded.get_len() << "\n";






//    BigInt x = 110;
//    BigInt y = 10;
//    (x/y).print();


    return 0;
}
