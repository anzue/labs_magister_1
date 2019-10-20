#include <iostream>

#include "sha_224.cpp"
#include "bigint.h"

using namespace std;

int BigInt::DEBUG = 0;


BigInt get_random_big_int(int bytes){
    BigInt res = 0;
    for(int i=0;i<bytes;++i){
        res = res * BigInt((unsigned int)mod_part) + BigInt((unsigned int)rand()%mod_part);
    }
    return res;
}



bool is_prime(BigInt val){

    for(int i=0;i<10;++i){
        BigInt a = get_random_big_int(val.get_len());
        //a.print();
        if(a >= val || a == BigInt(0)){
            --i;continue;
        }
        if(! (binpow(a, val - one,val) == one)){
            return false;
        }
    }
    return true;
}

BigInt get_random_prime(int bytes){
    //"203391339108018642348853042914050827989603460621" one of primes found for 80 bit
    BigInt q = get_random_big_int(bytes);
    if(q%two == BigInt(0))
        q += one;

    BigInt hungred = 10;

    while(!is_prime(q)){
        q += two;

    }
    return q;
}

BigInt get_random_lesser(const BigInt& x){
    int bytes = x.get_len();
    BigInt res(0);
    do{
        res = get_random_big_int(bytes);
    }while(res >= x);
    return res;
}

void parametr_generation(BigInt& p,BigInt& q,BigInt& g)
{


    //    int L = 1024; // key length
    //    int N = 160;  // N - modulo

        int L = 24*4; // key length
        int N = 16*4;  // N - modulo

//    int L = 16; // key length
//    int N = 8;  //N - modulo

    q = get_random_prime(N/8);

    BigInt pdivq = get_random_big_int(L/8) / q;
    while( !is_prime( pdivq * q + one)){
        pdivq += 1;
    }
    p = q*pdivq + one;

    /* BigInt h = p;
    while( h>= p-two){
        h = get_random_big_int(L/8);
    }*/
    BigInt h = 2;

    g = binpow(h,pdivq,p);

}

void user_key(const BigInt& p, const BigInt& q,const BigInt& g,BigInt& x,BigInt& y){
    x = get_random_lesser(q);
    y = binpow(g,x,p);
}

void signature(const BigInt& p, const BigInt& q,const BigInt& g,const BigInt& x, const BigInt& coded, BigInt& r,BigInt& s){


    while(true){
        BigInt k = get_random_lesser(q);
       // k = 15;//for test only
        r = binpow(g,k,p)%q;
        if(r == zero) continue;
        s = (rev(k,q)*(coded + x*r))%q;
        if(s == zero) continue;
        return;
    }
}


bool verify_signature(const BigInt& p, const BigInt& q,const BigInt& g,const BigInt& y, const BigInt& coded, BigInt& r,BigInt& s){
    BigInt w = rev(s,q);
    BigInt u1 = (coded * w) % q;
    BigInt u2 = (r*w) % q;
    BigInt v = (binpow(g,u1,p)*binpow(y,u2,p))%p%q;


    cout << "w  = ";w.print();
    cout << "u1 = ";u1.print();
    cout << "u2 = ";u2.print();
    cout << "v  = ";v.print();
    cout << "r  = ";r.print();
    if(v == r){
        return true;
    }
    return false;
}


void test_simple_dsa() { // by https://www.di-mgt.com.au/public-key-crypto-discrete-logs-4-dsa.html
    BigInt p,q,g;
    p = 283;
    q = 47;
    g = 60;
    cout << "Chosen prime q: ";q.print(cout,BigInt::PrintType::DEC);
    cout << "Chosen prime p: ";p.print(cout,BigInt::PrintType::DEC);
    cout << "Chosen power g: ";g.print(cout,BigInt::PrintType::DEC);
    cout << std::endl;

    BigInt x;//private key aka A
    BigInt y;//public key aka a

    x = 24;
    y = 158;

    cout << "x = ";x.print(cout,BigInt::PrintType::DEC);
    cout << "y = ";y.print(cout,BigInt::PrintType::DEC);
    cout << std::endl;

    BigInt coded = 41;

    BigInt r,s;// signature
    signature(p,q,g,x,coded,r,s);
    cout << "Signature(r,s)\n";
    r.print(cout,BigInt::PrintType::DEC);
    s.print(cout,BigInt::PrintType::DEC);
    cout << std::endl;

    bool verified_signature = verify_signature(p,q,g,y,coded,r,s);
    cout << "verify signature is " << (verified_signature?"True":"False") <<"\n";
    cout << std::endl;

}


int main(){


    /*test_simple_dsa();
    return 0;*/

    srand(1);

    string code = "The quick brown fox jumps over the lazy dog";
    string coded_as_str = SHA2::SHA2_main(code);
    cout << "After SHA-224\n";
    cout << coded_as_str;
    BigInt coded = BigInt(coded_as_str,16);
    cout << "Coded len = " << std::dec << 8*coded.get_len() << "\n";
    cout << std::endl;

    BigInt p,q,g;
    parametr_generation(p,q,g);

    cout << "Chosen prime q: ";q.print(cout,BigInt::PrintType::HEX);
    cout << "Chosen prime p: ";p.print(cout,BigInt::PrintType::HEX);
    cout << "Chosen power g: ";g.print(cout,BigInt::PrintType::HEX);
    cout << std::endl;

    BigInt x;//private key
    BigInt y;//public key
    user_key(p,q,g,x,y);
    cout << "x = ";x.print(cout,BigInt::PrintType::HEX);
    cout << "y = ";y.print(cout,BigInt::PrintType::HEX);
    cout << std::endl;

    BigInt r,s;// signature
    signature(p,q,g,x,coded,r,s);
    cout << "Signature(r,s)\n";
    r.print(cout,BigInt::PrintType::HEX);
    s.print(cout,BigInt::PrintType::HEX);
    cout << std::endl;

    bool verified_signature = verify_signature(p,q,g,y,coded,r,s);
    cout << "verify signature is " << (verified_signature?"True":"False") <<"\n";
    cout << std::endl;
}
