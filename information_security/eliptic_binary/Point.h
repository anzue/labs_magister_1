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
    U z;
public:
    bool is_on_curve(){

        return y*y*z + x*y*z == x*x*x + A*x*z*z + B*z*z*z;
    }

    Point(U tx,U ty,U tz){ x = tx;y = ty;z=tz;}
    Point(const Point& p) {x = p.x; y = p.y;z=p.z;}
    Point(){ x = U(0);y = U(0);z = U(0);}

    Point operator+(Point p)
    {
        Point res;
        /*
        U l1 = x*p.z;
        U l2 = p.x*z;
        U l3 = l1+l2;
        U l4 = l1*l1;
        U l5 = l2*l2;
        U l6 = l4+l5;
        U l7 = y*p.z*p.z;
        U l8 = p.y*z*z;
        U l9 = l7+l8;
        U l10 = l3*l9;
        res.z = z*p.z*l6;
        res.x = l1*(l8+l5)+l2*(l7+l4);
        res.y = (l1*l10 + l7*l6)*l6 + (l10+res.z)*res.x;*/

        res.x = sqr((y + p.y) * inv(x + p.x)) + (y + p.y)*inv(x+p.x) + x + p.x + A;
        res.y = ((y + p.y) * inv(x + p.y)) * (x + p.x) + res.x + y;
        res.z = p.z;

        return res;
    }

    Point Double()
    {
         Point res;
         U l1 = x*x;
         U l2 = l1 + y;
         U l3 = x*z;
         res.z = l3*l3;
         U l5 = l2*res.z;
         res.x = l2*l2 + l3 + A*res.z;
         res.y = (res.z + l5)*res.x + l1*l1*res.z;
         return res;
    }

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
                    res = res.Double();
                else
                    res = res + p;
                x = x - 1;
            }
            x = x / 2;
            p = p.Double();
        }
        return res;
    }

    friend ostream & operator << (ostream &out, const Point &c);
};
#endif // POINT_H
