#pragma once
#include "bigint.h"

BigInt crypt(const BigInt& crypt_message, const BigInt& public_key, const BigInt& prime_multiplication);
