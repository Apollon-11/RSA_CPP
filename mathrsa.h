#pragma once
#include "bigint.h"

class math {
private:
    void extended_euclid(const BigInt& a, const BigInt& b, BigInt* x, BigInt* y, BigInt* d);
    BigInt mul_mod(const BigInt& x, const BigInt& y, const BigInt& m);
public:
    BigInt pow_mod(const BigInt& x, const BigInt& n, const BigInt& m);
    BigInt inverse(const BigInt& a, const BigInt& n);
};
