#include <iostream>
#include "bigint.h"
using namespace std;

#include "eliptic.h"
#define U BigInt

U extEuclid(U a,U b, U &x, U & y , U mod){
    if(a == 0){
        x = 0;y=1;
        return b;
    }
    U x1,y1;
    U d = extEuclid(b%a,a,x1,y1,mod);
    x = ((mod*mod+y1) - (b/a)*x1)%mod;
    y = x1;
    return d;
}


U Montgomeri(U a,U b,U n ){

    if(n%two == 0){
        cout <<"n must be odd\n";
        return 0;
    }
    U r = 1;
    while(r<n)
        r*=2;

    U r_inv , n_;
    extEuclid(r,n,r_inv,n_,n);

    U k = (r*r_inv - one)/n;

    U a_ = (a*r)%n;
    U b_  = (b*r)%n;

    U x = (a_ * b_)%n;
    U s = (x*k)%r;
    U t = x+s*n;
    U u = t/r;
    U c_;
    if(u<n)c_ = u;
    else c_ = u - n;
    U c = (c_  * r_inv)%n;
    return c;
}

U REDC(U T,U R,U N){
    // find t *r^-1 mod n
    U n_ = R - rev(N,R);
    U m = ((T % R)*n_)%R;
    U t = (T+ m*N)/R;
    if(t>=N)
        t-=N;

    return t;
}


U binpow_mont(U x,U pow,U mod){
    U r = 1;
    while(pow>zero){
        if(pow%two==1)
            r = Montgomeri(r,x,mod);
        x = Montgomeri(x,x,mod);
        pow=pow/two;
    }
    return r;
}

U Diffie(){

    U g = string("13");//open prime
    U p = string("91") ; // mod
    U a = string("12");//alice key;
    U b = string("19");//bob key


    U A = modpow(g,a,p);//Alice ->Bob
    U B = modpow(g,b,p);//Bob ->Alive
    U A_b = modpow(A,b,p); // Bob's res key
    U B_a = modpow(B,a,p); // Alice's res key

    cout<<"Bob   get key-";A_b.print(std::cout,BigInt::PrintType::DEC,2);cout<<"\n";
    cout<<"Alice get key-";B_a.print(std::cout,BigInt::PrintType::DEC,2);
}

int BigInt::DEBUG = false;

void test_fib()
{
    BigInt::DEBUG = false;
    BigInt a = 0,b=1,c;

    for(int i=0;i<1000;++i){
        c = a+b;
        a = b;b=c;
    }
    a.print();b.print();c.print();

    /*
     * fib1000 =
     43466557686937456435688527675040625802564660517371780402481729089536555417949
     05189040387984007925516929592259308032263477520968962323987332247116164299644
     0906533187938298969649928516003704476137795166849228875
     */

}

int main(){
    test_fib();
}

