#include "key_gen.h"
#include "mathrsa.h"

math kgrsa;

BigInt Key_Gen(const BigInt& prime1, const BigInt& prime2, const BigInt& open_key) {
    BigInt function_euler = (prime1 - BigInt(1)) * (prime2 - BigInt(1));
    BigInt inv = kgrsa.inverse(open_key, function_euler);
    BigInt zero(0);
    if (inv == zero) return inv; // no inverse
    while (inv < zero) inv = inv + function_euler;
    inv = inv % function_euler;
    return inv;
}

