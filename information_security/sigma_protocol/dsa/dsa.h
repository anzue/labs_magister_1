#ifndef DSA_H
#define DSA_H


#include "bigint.h"
#include "../sha/sha_224.h"

void dsa_sign(string mess, string key,
              BigInt &coded,BigInt &r,BigInt &s,
              BigInt &p,BigInt &q, BigInt &g,BigInt &y);

bool dsa_check(
        BigInt coded,BigInt r,BigInt s,
        BigInt &p,BigInt &q, BigInt &g,BigInt &y);


#endif // DSA_H
