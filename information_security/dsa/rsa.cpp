#include "bigint.h"
#include "sha_224.h"

void test_rsa(){



    string code = "The quick brown fox jumps over the lazy dog";
    string coded_as_str = SHA2::SHA2_main(code);
    cout << "After SHA-224\n";
    cout << coded_as_str;
    BigInt m = BigInt(coded_as_str,16);
    cout << "Coded len = " << std::dec << 8*m.get_len() << "\n";
    cout << std::endl;


    int p_len = 10*16;
    int q_len = 10*16;

    srand(1);

    BigInt p = get_random_prime(p_len/8);
    BigInt q = get_random_prime(q_len/8);
    BigInt n = p*q;
    BigInt lambda = ((p-one)*(q-one)) / gcd(p-one,q-one); // secret

    cout << " p =         ";p.print(cout,BigInt::PrintType::HEX);
    cout << " q =         ";q.print(cout,BigInt::PrintType::HEX);
    cout << " n =         ";n.print(cout,BigInt::PrintType::HEX);
    cout << " lambda(n) = ";lambda.print(cout,BigInt::PrintType::HEX);


    /*
    BigInt e;
    do{
        e = get_random_lesser(lambda);
    }while (gcd(e,lambda) > one);

    */
    BigInt e = 65537;


    cout << "e =          ";e.print(cout,BigInt::PrintType::HEX);

    BigInt d = rev_Euclid(e,lambda); // private key exponent

    cout << "d =          ";d.print(cout,BigInt::PrintType::HEX);


    cout <<"de % lambda  ";((d*e)%lambda).print(cout,BigInt::PrintType::HEX);


    cout << "Public key = \n";
    n.print(cout,BigInt::PrintType::HEX);
    e.print(cout,BigInt::PrintType::HEX);
    cout << "Public key end\n";



    cout << "m =      ";(m%n).print(cout,BigInt::PrintType::HEX);

    BigInt c = binpow(m,e,n);

    cout <<"c =       ";c.print(cout,BigInt::PrintType::HEX);

    BigInt decoded_m = binpow(c,d,n);

    cout << "decoded  ";decoded_m.print(cout,BigInt::PrintType::HEX);
}
