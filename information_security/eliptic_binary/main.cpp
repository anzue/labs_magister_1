#include <iostream>
#include "point.h"
#include "BinaryPolynomial.h"
#include <time.h>
#include <assert.h>

using namespace std;

Bits get_random_bits(){
    Bits res;
    for(int i=0;i < BinaryPolynomial::n / 8;++i){
        res = (res << 8) ^ Bits(rand()%(1<<8));
    }
    return res;
}

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
    BinaryPolynomial x(get_random_bits()),y(get_random_bits());


    cout << "x * y = " << x*y
         << "y * x = " << y*x;
    assert(x * y == y*x);
}


Point generate_point_test(){
    // y^2 + y * x + (x^3 + A*x + B) = 0

    int k;
    BinaryPolynomial x,y;
    do{
        x = get_random_bits();
        k = get_square_solution(x,x*x*x+A*x*x+B,y);
    }while( k <= 0);

    Point res_point = Point(x,y);

    cout << res_point;
    cout << "res count " << k <<"\n";
    cout << "On curve " << res_point.is_on_curve() << "\n\n";

    return res_point;
    // cout << res_point;

}

void test_math(){
    srand(time(0));
    Point p1 = generate_point_test();
    Point p2 = generate_point_test();
    Point p3 = generate_point_test();

    Point p12 = p1+p2;
    Point p23 = p2+p3;
    Point p13 = p1+p3;

    cout << p12 + p3 <<"\n"  <<
            p1 + p23 << "\n" <<
            p13 + p2 <<"\n";

}

void test_mul_point1(int v1 = (rand() % 12345),int v2 = rand()%54321){
    Point a = generate_point_test();

    Point a1 = (a*v1)*v2;
    Point a2 = (a*v2)*v1;
    Point a3 = (a*(v1*v2));
    Point a4 = (a.operator*<1000>(std::bitset<1000>(v1*v2)));

    cout << a1 << a2 << a3 << a4 <<"\n";
}

int main(){
    srand(time(0));

    test_mul_point1();
}
