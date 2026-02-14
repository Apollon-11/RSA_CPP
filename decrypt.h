#pragma once
#include "bigint.h"

BigInt decrypt(const BigInt& crypt_message, const BigInt& secret_key, const BigInt& prime_multiplication);
BigInt decrypt_crt(const BigInt& c, const BigInt& d, const BigInt& p, const BigInt& q);
