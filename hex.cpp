#include "hex.h"
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "bigint.h"

BigInt text_to_number(const std::string& text) {
    BigInt result = 0;
    for (unsigned char c : text) result = (result * 256) + BigInt((long long)c);
    return result;
}
static std::string dectohex(BigInt num) {
    if (num.is_zero()) return "0";

    const char* digits = "0123456789ABCDEF";
    std::string result;
    BigInt zero(0);
    BigInt sixteen(16);
    while (num > zero) {
        BigInt rem = num % sixteen;
        int digit = rem.get_digit(0);
        result = digits[digit] + result;
        num = num / sixteen;
    }
    if (result.length() % 2 != 0) result = "0" + result;
    return result;
}

std::string number_to_text(const BigInt& num) {
    if (num.is_zero()) return std::string();
    std::string hex = num.to_hex();
    if (hex.length() % 2) hex = "0" + hex;
    std::string result;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byte_str = hex.substr(i, 2);
        char c = static_cast<char>(stoi(byte_str, nullptr, 16));
        result += c;
    }
    while (!result.empty() && result.back() == '\0') result.pop_back();
    return result;
}
