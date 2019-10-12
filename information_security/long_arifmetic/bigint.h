#ifndef NEW_BIG_INT_H
#define NEW_BIG_INT_H

#include<iostream>
#include<iomanip>
#include<cstring>
#include<fstream>
#include<bitset>
#include<sstream>
#include <bits/stdc++.h>


using namespace std;

typedef unsigned int Value_type;
const int type_size = 8*sizeof(Value_type);
const int type_value_size = type_size/2;
const int mod_part = 1 << (type_size/2);
const int start_size = 2;


class BigInt
{
private:
    Value_type* values;
    int values_size;
    int len;
public:
    static int DEBUG;

    void init(int size);
    BigInt():values(nullptr),values_size(1),len(0){}
    BigInt(Value_type val);
    BigInt(const BigInt& old);
    BigInt(const string& old);

    void resize(int new_size);

    void normalize(){resize(len);}
    void update_len();
    void upscale(int new_size);

    inline Value_type get_value_at(int pos)const{ return (pos < 0 || pos >= len)? 0 : values[pos];}

    enum PrintType{
        HEX,DEC,BIN
    };

    void print(ostream& str = cout, PrintType type = PrintType::DEC,int padding = 0)const;

    BigInt operator=(const BigInt& old);
    BigInt operator=(const int& old);

    BigInt operator=(const string& old);

    bool operator==(const BigInt& a) const;
    bool operator==(const int& a) const;

    bool operator<(const BigInt& a) const;
    bool operator>(const BigInt& a) const;
    bool operator<=(const BigInt& a) const;
    bool operator>=(const BigInt& a) const;

    BigInt div2();

    BigInt operator+=(const BigInt& a);
    BigInt operator+(const BigInt& a) const;
    BigInt operator*=(const BigInt& a);
    BigInt operator*(const BigInt& a) const;
    BigInt operator/=(const BigInt& a);
    BigInt operator/(const BigInt& a) const;
    BigInt operator-=(const BigInt& a);
    BigInt operator-(const BigInt& a) const;
    BigInt operator%(const BigInt& a);

    operator bool() const{return !((*this) == BigInt(0));}
};
const BigInt
            zero = 0,
            one = 1,
            two = 2;


#endif // NEW_BIG_INT_H
