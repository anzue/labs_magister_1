#ifndef BINARY_POLYNOMAIL_H
#define BINARY_POLYNOMAIL_H

#include<bitset>
#include<iostream>

using std::bitset;
using std::ostream;

typedef std::bitset<167> Bits;

// polynomials in binary field modulo x^167 + x^6 + 1
class BinaryPolynomial
{
public:
    const static int n = 167; // degree of polynomial
    const static int a = 6,
                     b = 0; // coefs to replace n with

    const static char new_line = '\n';
    const static int print_values_count = n;

    Bits bits;

public:
    BinaryPolynomial(int val = 0):bits(val){}
    BinaryPolynomial(std::string val):bits(val){}
    BinaryPolynomial(const BinaryPolynomial& original):bits(original.bits){}
    BinaryPolynomial(Bits original_bits):    bits(original_bits){}

    BinaryPolynomial operator+(const BinaryPolynomial& value) const;
    BinaryPolynomial operator-(const BinaryPolynomial& value) const;
    BinaryPolynomial operator*(const BinaryPolynomial& value) const;


    //this is inverse
    BinaryPolynomial operator~() const;
    bool operator==(const BinaryPolynomial& value) const;

    Bits get_bits() const{ return bits; }

    friend ostream & operator<< (ostream &out, const BinaryPolynomial &c);
    friend BinaryPolynomial extEuclid(BinaryPolynomial x,BinaryPolynomial y,BinaryPolynomial &a,BinaryPolynomial &b);
    friend BinaryPolynomial sqrt(const BinaryPolynomial& value);
    friend BinaryPolynomial inv(const BinaryPolynomial& value);
    friend BinaryPolynomial trace(const BinaryPolynomial& value);
    friend BinaryPolynomial half_trace(const BinaryPolynomial& value);
    friend int get_square_solution(BinaryPolynomial u, BinaryPolynomial w, BinaryPolynomial& y);
};


#endif // BINARY_POLYNOMAIL_H
