#include <iostream>
#include "point.h"
#include "BinaryPolynomial.h"
#include <time.h>
#include <assert.h>
#include "diffie_hellman.h"

using namespace std;

void test_euclid(){
    BinaryPolynomial x = 65436544;
    BinaryPolynomial y = 63213453;

    BinaryPolynomial a,b;

    cout << extEuclid(x,y,a,b);


    cout << "x = " << x;
    cout << "y = " << y;
    cout << "a = " << a;
    cout << "b = " << b;

    cout << a*x + b*y ;

}

void test_reverse(){
    BinaryPolynomial x = 43547832LL;

    BinaryPolynomial a = inv(x);

    cout << "x =    " << x;
    cout << "x^-1 = " << a;
    cout << "x * x^-1 = " << x*a;
    cout << "x^-1 * x = " << a*x;

    //  cout << a << x << a + x << a*x;
}

void test_sqrt(){
    BinaryPolynomial x = 146434120;
    BinaryPolynomial y = sqrt(x);

    //  cout << x << " = " << y*y <<"\n";

}

void test_mul(){
    BinaryPolynomial x,y;
    get_random_bits<BinaryPolynomial::n>(x.bits);
    get_random_bits<BinaryPolynomial::n>(y.bits);

    cout << "x * y = " << x*y
         << "y * x = " << y*x;

    assert(x * y == y*x);
}




void test_math(){
    srand(time(0));
    Point p1 = generate_point();
    Point p2 = generate_point();
    Point p3 = generate_point();

    Point p12 = p1+p2;
    Point p23 = p2+p3;
    Point p13 = p1+p3;

    cout << p12 + p3 <<"\n"  <<
            p1 + p23 << "\n" <<
            p13 + p2 <<"\n";

}

void test_mul_point1(int v1 = (rand() % 12345),int v2 = rand()%54321){
    Point a = generate_point();

    Point a1 = (a*v1)*v2;
    Point a2 = (a*v2)*v1;
    Point a3 = (a*(v1*v2));
    Point a4 = (a.operator*<1000>(std::bitset<1000>(v1*v2)));

    cout << a1 << a2 << a3 << a4 <<"\n";
}

int main(){
    srand(time(0));
    diffie_hellman();
}
