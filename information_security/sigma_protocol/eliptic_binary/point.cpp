#include "Point.h"

ostream & operator << (ostream &out, const Point &point){
    out << "x = " << point.x;
    out << "y = " << point.y;
    out << "on curve = " << point.is_on_curve();
    out << "\n";
}
/*
template <size_t len>
void get_random_bits(bitset<len> &inp){
    for(int i=0;i <len/8 + 1;++i){
        inp = (inp << 8) ^ std::bitset<len>(rand()%(1<<8));
    }
}*/


Point generate_point(){
    int k;
    BinaryPolynomial x,y;
    do{
        get_random_bits<BinaryPolynomial::n>(x.bits);
        k = get_square_solution(x,x*x*x+A*x*x+B,y);
    }while( k <= 0);
    Point res_point = Point(x,y);
    return res_point;
}
