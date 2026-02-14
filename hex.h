#pragma once
#include <string>
#include "bigint.h"

BigInt text_to_number(const std::string& text);
std::string number_to_text(const BigInt& num);
