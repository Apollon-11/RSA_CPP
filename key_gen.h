#pragma once
#include "mathrsa.h"
#include <string>
#include "bigint.h"

BigInt Key_Gen(const BigInt& prime1, const BigInt& prime2, const BigInt& open_key);

