#include "mathrsa.h"

extern math crsa;

BigInt crypt(const BigInt& crypt_message, const BigInt& public_key, const BigInt& prime_multiplication) {
    return crsa.pow_mod(crypt_message, public_key, prime_multiplication);
}
