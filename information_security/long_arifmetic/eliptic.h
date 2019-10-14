#ifndef ELIPTIC_H
#define ELIPTIC_H
#include "bigint.h"
typedef BigInt U;
#include <iostream>
using namespace std;

#define TEST

#ifdef TEST
U modpow(U a,U p,U m){
    U r = 1;
    U zero = 0;
    U two = 2;
    while(p>zero){

        a.print();p.print();r.print();m.print();cout<<"\n";

        if(p%two)
            r = (r * a)%m;
        a = (a * a)%m;
        p /= two;

        a.print();p.print();r.print();m.print();cout<<"\n";


    }
    return r;
}


U rev(U x,U mod){
    return modpow(x,mod-two,mod);
}
#endif


class Point
 {
public:
      U x;
      U y;
      static U a;
      static U b;
      static U mod;

      Point(U tx,U ty){ x = tx;y = ty;}
      Point(const Point& p) {x = p.x; y = p.y;}
      Point(){ x = U(0);y = U(0); }

     Point operator+(Point p1)
     {
         Point p2;
         U dy = mod + p1.y - y;
         U dx = mod + p1.x - x;

         if (dx >= mod)
             dx -= mod;
         if (dy >= mod)
             dy -= mod;

         U m = (dy * rev(dx,mod)) % mod;
         p2.x = (m * m  + mod + mod - x - p1.x) % mod;
         p2.y = (mod * m + m * (x - p2.x) - y) % mod;

         return p2;
     }

     Point Double()
     {
         Point p2;

         U dy = 3 * x * x + a;
         U dx = 2 * y;

         U m = (dy * rev(dx,mod)) % mod;
         p2.x = (m * m +mod+mod- x - x) % mod;
         p2.y = (m * (mod + x - p2.x) + mod - y) % mod;

         return p2;
     }

     Point operator*(U x)
     {
         Point p = *this;
         Point res = p;
         x = x - one;
         while (x != 0)
         {
             if ((x % two) != 0)
             {
                 if ((res.x == p.x) || (res.y == p.y))
                     res = res.Double();
                 else
                     res = res + p;
                 x = x - one;
             }
             x = x / two;
             p = p.Double();
         }
         return res;
     }

     static void printCurve(){
#ifdef TEST
         cout<<"Curve - y^2  = x^3 + "<<a<<"x + "<<b<<"\n";
         cout<<"p = "<<mod<<"\n";
#else
        cout<<"Curve - y^2  = x^3 + ";a.print();cout<<"x + ";b.print();cout<<"\n";
        cout<<"p = ";mod.println();
#endif
     }
     void printPoint(){
#ifdef TEST
         //cout<<"Point -
         cout<<"("<<x<<","<<y<<")\n";
#else
         cout<<"Point - (";x.print();cout<<",";y.print();cout<<")\n";

    #endif
     }
};



#endif // ELIPTIC_H
