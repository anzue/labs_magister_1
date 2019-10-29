#include "BinaryPolynomial.h"
#include <assert.h>



ostream & operator << (ostream &out, const BinaryPolynomial &c){

    if(BinaryPolynomial::print_values_count < BinaryPolynomial::n){
        for(int i=0;i<BinaryPolynomial::print_values_count-1;++i){
            out << c.bits[i];
        }
        out << BinaryPolynomial::new_line;
        return out;
    }

    return out << c.bits << BinaryPolynomial::new_line;
}


Bits mul(Bits a,Bits b){
    Bits res = 0;
    Bits tmp = 0;
    Bits mask;
    mask.set();

    for(int i=0;i < BinaryPolynomial::n;++i){
        if(a[i]){
            tmp = b;
            res ^= (tmp & mask) << i;
            tmp >>= (BinaryPolynomial::n-i);
            while( tmp != 0){
                res^=(tmp << BinaryPolynomial::a) ^ (tmp << BinaryPolynomial::b);
                // redunant but needed in general
                tmp = (tmp >> (BinaryPolynomial::n-BinaryPolynomial::a)) ^
                      (tmp >>(BinaryPolynomial::n-BinaryPolynomial::b));
            }
        }
        mask >>=  1;
    }
    return res;
}

Bits extEuclid(Bits x,Bits y,Bits &a,Bits &b,
               int x_pos/* = BinaryPolynomial::n-1*/,
               int y_pos/* = BinaryPolynomial::n-1*/){

    while(x_pos >= 0 && x[x_pos] == 0) --x_pos;
    while(y_pos >= 0 && y[y_pos] == 0) --y_pos;

    if( x_pos < 0 ){
        a = 0;
        b = 1;
        return y;
    }

    Bits a1,b1;
    Bits d;
    Bits tmp; // this is  [y/x]
    tmp.reset();


    while(y_pos >= x_pos){
        if(y[y_pos]){
            tmp.flip(y_pos - x_pos);
            y = y ^ (x << (y_pos - x_pos));
        }
        --y_pos;
    }

    d = extEuclid( y,x, a1,b1,y_pos,x_pos);

    b = a1;
    a = b1 ^ mul(tmp,a1);

//    std::cout << "check\n";
//    std::cout << a  << " * " << x << " + " << b << " * " << (y ^ mul(tmp,x)) << " = "
//              << BinaryPolynomial(a)*x + BinaryPolynomial(b)*(y ^ mul(tmp,x)) << " " << d <<"\n";

    return d;


}

BinaryPolynomial extEuclid(BinaryPolynomial x,
                           BinaryPolynomial y,
                           BinaryPolynomial &a,
                           BinaryPolynomial &b){

    Bits a_tmp,b_tmp,d;



    d = extEuclid(x.bits,y.bits,a_tmp,b_tmp,BinaryPolynomial::n-1,BinaryPolynomial::n-1);

    a = BinaryPolynomial(a_tmp);
    b = BinaryPolynomial(b_tmp);
    return BinaryPolynomial(d);

}


BinaryPolynomial BinaryPolynomial::operator+(const BinaryPolynomial& value) const{
    return BinaryPolynomial(bits ^ value.bits);
}
BinaryPolynomial BinaryPolynomial::operator-(const BinaryPolynomial& value) const{
    return BinaryPolynomial(bits ^ value.bits);
}


//this is inverse
BinaryPolynomial BinaryPolynomial::operator~() const{
    // TODO extended euclid here
    BinaryPolynomial tmp1(*this);
    BinaryPolynomial tmp2(*this);
    return BinaryPolynomial(~bits);
}
bool BinaryPolynomial::operator==(const BinaryPolynomial& value) const{
    return bits == value.bits;
}

BinaryPolynomial BinaryPolynomial::operator*(const BinaryPolynomial& value) const{

    return mul(bits,value.bits);

    Bits res = 0;
    Bits tmp = 0;
    Bits mask;
    mask.set();

    for(int i=0;i < n;++i){
        if(bits[i]){

            tmp = value.bits;
            res ^= (tmp & mask) << i;
            tmp >>= (n-i);

            // TODO this can be incorect when tmp << a is overflow

            while( tmp != 0){
                res^=(tmp << a) ^ (tmp << b);
                // redunant but needed in general
                tmp = (tmp >> (n-a)) ^ (tmp >>(n-b));
            }
        }
        mask >>=  1;
    }
    return BinaryPolynomial(res);
}

BinaryPolynomial sqrt(const BinaryPolynomial& value){
    // heavily depended on polynomial
    BinaryPolynomial res = 0;
    BinaryPolynomial tmp(value);
    Bits mask = 0;

    for(int i=0;i<BinaryPolynomial::n;++i){
        if(tmp.bits[i]){
            if(i%2 == 0){
                res.bits.flip(i/2);
            }
            else{
                tmp.bits.flip(i);
                res.bits.flip((i + BinaryPolynomial::n)/2);
                if( i +  6 < BinaryPolynomial::n){
                    tmp.bits.flip(i + 6);
                }
                else{
                    res.bits.flip((i+6-BinaryPolynomial::n + 0)/2);
                    res.bits.flip((i+6-BinaryPolynomial::n + 6)/2);
                }
            }
        }
    }
    //TODO can be removed
    assert(value == res*res);
    return res;
}

BinaryPolynomial inv(const BinaryPolynomial& value){
    BinaryPolynomial x(value);
    int pos = BinaryPolynomial::n-1;
    while(pos >= 0 && value.bits[pos] == 0){
        --pos;
    }
    if(pos < 0){
        return 0;
    }
    int shift = BinaryPolynomial::n - pos;
    x = (value.get_bits() << shift);
    x.bits.flip(BinaryPolynomial::a);
    x.bits.flip(BinaryPolynomial::b);
    BinaryPolynomial a,b,d;
    d = extEuclid(value, x,a,b);
    //TODO overflow ??
    return  a + (b.bits << shift);
}

BinaryPolynomial trace(const BinaryPolynomial& value){
    Bits res = value.bits;
    for(int i=0;i<BinaryPolynomial::n-1;++i){
        res = mul(res,res) ^ value.bits;
    }
    return BinaryPolynomial(res);
}

BinaryPolynomial half_trace(const BinaryPolynomial& value){
    Bits res = value.bits;
    Bits tmp;
    for(int i=0;i<(BinaryPolynomial::n-1)/2;++i){
        tmp = mul(res,res);
        res = mul(tmp,tmp) ^ value.bits;
    }
    return BinaryPolynomial(res);
}

int get_square_solution(BinaryPolynomial u, BinaryPolynomial w, BinaryPolynomial& y){
    if( u == 0 ){
        y = sqrt(w);
        // w ^ 2 ^(n-1) works here too ?
        return 1;
    }
    if(w == 0){
        y = 0;
        return 2;
    }

    BinaryPolynomial v = w*inv(u)*inv(u);
    BinaryPolynomial tra = trace(v);
    if(tra == 1){
        return 0;
    }
    BinaryPolynomial half_tra = half_trace(v);

    //std::cout << u << w << v  << tra << half_tra;

    y = half_tra*u;

    //std::cout << y*y + y*u + w;

    return 2;
}

