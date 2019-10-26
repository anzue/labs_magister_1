#include <iostream>
#include "point.h"
#include "BinaryPolynomial.h"
#include <time.h>

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


Point generate_point_test(){
    // y^2 + y * x + (x^3 + A*x + B) = 0
    /*
    BinaryPolynomial z = 1;
    BinaryPolynomial y = 0;
    BinaryPolynomial C = x*x*x + A*x+B;
    int cur_bit;
    int c_x[] = {0,1,0,1};
    int c_y[] = {0,0,1,1};

    for(int i=0;i<BinaryPolynomial::n;++i){
        cur_bit = 0;
        for(int j=1;j<i;++j){
            cur_bit ^= (x.bits[j]^y.bits[j])*y.bits[i-j];
        }
        bool ok = false;
        for(int y_cand=0;y_cand<2;++y_cand){
            if( y_cand)
        }


    }*/
    Bits x_bits;
    srand(time(0));
    for(int i=0;i < BinaryPolynomial::n / 8;++i){
        x_bits = (x_bits << 8) ^ Bits(rand()%(1<<8));
    }

    BinaryPolynomial x(x_bits);
    BinaryPolynomial z = 1;
    BinaryPolynomial y = 0;
    int k = get_square_solution(x,x*x*x+A*x+B,y);

    Point res_point = Point(x,y,z);
    cout << "res count " << k <<"\n";
    cout << "On curve " << res_point.is_on_curve() << "\n";

   // cout << res_point;

}

void test_math(){
    //test_euclid();return 0;
    Point p1 = generate_point_test();
    Point p2 = generate_point_test();
    Point p3 = generate_point_test();
    cout << p1 <<"\n" << p2 <<"\n" << p3 <<"\n";

    Point p12 = p1+p2;
    Point p23 = p2+p3;
    Point p13 = p1+p3;

    cout << p12 << "\n" << (p2+p1) <<"\n";

    cout << p12 + p3 <<"\n"  <<
            p1 + p23 << "\n" <<
            p13 + p2 <<"\n";

}

int main(){
    test_math();
}
