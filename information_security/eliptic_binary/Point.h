#ifndef POINT_H
#define POINT_H

#define sqr(a) ((a)*(a))

#include <iostream>
using std::cout;

#include "BinaryPolynomial.h"

typedef BinaryPolynomial U;


const static U A = 1;
const static U B = std::string(
            "1101110111000111100111011101011001000110000100000010"\
            "0010111010110011111001000000101000110001010000010010"\
            "0110000111100011010010000110001010110101000001001111"\
            "10110101100"); //6EE3CEEB230811759F20518A0930F1A4315A827DAC

// y^2 + xy = x^3 +  A*x^2 + B
// y^2*z + xy*z = x^3 +  A*x^2*z + B*z^3
/*

*/


class Point
{
    U x;
    U y;
public:
    bool is_on_curve() const{
        return y*y + x*y == x*x*x + A*x*x + B;
    }

    Point(U tx,U ty){ x = tx;y = ty;}
    Point(const Point& p) {x = p.x; y = p.y;}
    Point(){ x = U(0);y = U(0);}

    Point add(Point p){
        U l = (y + p.y) * inv(x+p.x);
        U x3 = l*l + l + x + p.x + A;
        U y3 = l*(x + x3) + x3 + y;
        return Point(x3,y3);
    }

    Point double_point()
    {
        Point res;
        U m = x + y*inv(x);
        res.x = m*m + m + A;
        res.y = x*x + (m + 1)*res.x;
        return res;
    }


    Point operator+(Point p)
    {
        if( !(x == p.x) ){
            return add(p);
        }
        if( y == p.y ){
            return double_point();
        }
        return Point(0,0);
    }

    // multiply by long long val
    Point operator*(long long x)
    {
        Point p = *this;
        Point res = p;
        x = x - 1;

        while (x != 0)
        {
            if ((x % 2) != 0)
            {
                if ((res.x == p.x) || (res.y == p.y))
                    res = res.double_point();
                else
                    res = res + p;
                x = x - 1;
            }
            x = x / 2;
            p = p.double_point();
        }
        return res;
    }

    // multiply by binary int represented as bitset
    template<int number_length>
    Point operator*(std::bitset<number_length> x)
    {
        if(x == 0){
            return Point(0,0);
        }
        Point p = *this;
        Point res = p;

        int pos = 0;
        do{
            x.flip(pos);
        }while(x[pos++] == 1);

        pos = 0;
        for(;pos < number_length;++pos)
        {
            if(x[pos])
            {
                if ((res.x == p.x) || (res.y == p.y))
                    res = res.double_point();
                else
                    res = res + p;
            }
            p = p.double_point();
        }
        return res;
    }

    friend ostream & operator << (ostream &out, const Point &c);
};

template <size_t len>
void get_random_bits(bitset<len> &inp){
    for(int i=0;i <len/8 + 1;++i){
        inp = (inp << 8) ^ std::bitset<len>(rand()%(1<<8));
    }
}

Point generate_point();

#endif // POINT_H
