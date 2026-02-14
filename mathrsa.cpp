#include <iostream>
#include "mathrsa.h"
#include "bigint.h"
#include <vector>
#include <algorithm>

math crsa;

BigInt math::mul_mod(const BigInt& x, const BigInt& y, const BigInt& m) {
    return (x * y) % m;
}

BigInt math::pow_mod(const BigInt& x_in, const BigInt& n_in, const BigInt& m) {
    BigInt x = x_in;
    BigInt n = n_in;
    BigInt res = 1;
    while (n > 0) {
        if (n & 1)
            res = mul_mod(res, x, m);
        x = mul_mod(x, x, m);
        n >>= 1;
    }
    return res;
};

void math::extended_euclid(const BigInt& a_in, const BigInt& b_in, BigInt* x, BigInt* y, BigInt* d)
{
    BigInt a = a_in;
    BigInt b = b_in;
    BigInt q, r, x1, x2, y1, y2;
    if (b == BigInt(0)) {
        *d = a; *x = BigInt(1); *y = BigInt(0);
        return;
    }
    x2 = BigInt(1); x1 = BigInt(0); y2 = BigInt(0); y1 = BigInt(1);
    while (b > BigInt(0)) {
        q = a / b; r = a - q * b;
        *x = x2 - q * x1; *y = y2 - q * y1;
        a = b; b = r;
        x2 = x1; x1 = *x; y2 = y1; y1 = *y;
    }
    *d = a; *x = x2; *y = y2;
}

BigInt math::inverse(const BigInt& a, const BigInt& n)
{
    BigInt d, x, y;
    extended_euclid(a, n, &x, &y, &d);
	if (d == 1) return x;
	return 0;
};
