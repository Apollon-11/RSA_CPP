#include "bigint.h"
#include <iostream>
#include <algorithm>

BigInt::BigInt() : negative(false) { digits.push_back(0); }

BigInt::BigInt(long long n) {
    negative = (n < 0);
    if (negative) n = -n;
    if (n == 0) {
        digits.push_back(0);
    }
    else {
        while (n > 0) {
            digits.push_back(n % BASE);
            n /= BASE;
        }
    }
}

BigInt::BigInt(const std::string& s) {
    negative = false;
    digits.clear();
    int start = 0;
    if (!s.empty() && s[0] == '-') { negative = true; start = 1; }
    for (int i = (int)s.length() - 1; i >= start; i -= BASE_DIGITS) {
        int start_pos = std::max(start, i - BASE_DIGITS + 1);
        int len = i - start_pos + 1;
        int part = std::stoi(s.substr(start_pos, len));
        digits.push_back(part);
    }
    normalize();
}

void BigInt::normalize() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.empty()) {
        digits.push_back(0);
        negative = false;
    }
}

bool BigInt::is_zero() const { return digits.size() == 1 && digits[0] == 0; }

bool BigInt::operator==(const BigInt& other) const {
    return negative == other.negative && digits == other.digits;
}

bool BigInt::operator<(const BigInt& other) const {
    if (negative != other.negative) return negative;
    if (digits.size() != other.digits.size()) {
        return (digits.size() < other.digits.size()) ^ negative;
    }
    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            return (digits[i] < other.digits[i]) ^ negative;
        }
    }
    return false;
}

bool BigInt::operator>(const BigInt& other) const {
    return other < *this;
}

BigInt BigInt::operator+(const BigInt& other) const {
    if (negative != other.negative) {
        BigInt tmp = *this;
        tmp.negative = false;
        return other - tmp;
    }
    BigInt res;
    res.digits.clear();
    res.negative = negative;
    int carry = 0;
    size_t n = std::max(digits.size(), other.digits.size());
    for (size_t i = 0; i < n || carry; ++i) {
        int sum = carry;
        if (i < digits.size()) sum += digits[i];
        if (i < other.digits.size()) sum += other.digits[i];
        res.digits.push_back(sum % BASE);
        carry = sum / BASE;
    }
    res.normalize();
    return res;
}

BigInt BigInt::operator-(const BigInt& other) const {
    if (negative != other.negative) {
        BigInt tmp = other;
        tmp.negative = !tmp.negative;
        return *this + tmp;
    }
    if (*this < other) {
        BigInt res = other - *this;
        res.negative = !negative;
        return res;
    }
    BigInt res;
    res.digits.clear();
    res.negative = negative;
    int carry = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
        int sub = digits[i] - carry;
        if (i < other.digits.size()) sub -= other.digits[i];
        if (sub < 0) {
            sub += BASE;
            carry = 1;
        }
        else {
            carry = 0;
        }
        res.digits.push_back(sub);
    }
    res.normalize();
    return res;
}

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt res;
    res.digits.resize(digits.size() + other.digits.size(), 0);
    res.negative = negative != other.negative;
    for (size_t i = 0; i < digits.size(); ++i) {
        long long carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            long long cur = res.digits[i + j] +
                digits[i] * 1LL * (j < other.digits.size() ? other.digits[j] : 0) +
                carry;
            res.digits[i + j] = cur % BASE;
            carry = cur / BASE;
        }
    }
    res.normalize();
    return res;
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (other.is_zero()) throw std::runtime_error("Division by zero");
    BigInt a = *this;
    bool result_negative = (a.negative != other.negative);
    // work with absolute values
    a.negative = false;
    BigInt b = other;
    b.negative = false;

    if (a < b) return BigInt(0);

    // helper: shift left by k base-digits (multiply by BASE^k)
    auto shift_left = [&](const BigInt &x, int k) {
        BigInt r = x;
        if (x.is_zero()) return r;
        r.digits.insert(r.digits.begin(), k, 0);
        return r;
    };

    BigInt remainder = a;
    int n = (int)remainder.digits.size();
    int m = (int)b.digits.size();
    int shift = n - m;
    BigInt denom = shift_left(b, shift);

    BigInt quotient;
    quotient.digits.assign(shift + 1, 0);

    for (int i = shift; i >= 0; --i) {
        // binary search digit in [0, BASE-1]
        int low = 0, high = BASE - 1;
        int best = 0;
        while (low <= high) {
            int mid = (low + high) >> 1;
            BigInt prod = denom * BigInt((long long)mid);
            if (!(remainder < prod)) { best = mid; low = mid + 1; } else { high = mid - 1; }
        }
        if (best) {
            remainder = remainder - denom * BigInt((long long)best);
            quotient.digits[i] = best;
        }
        if (i > 0) {
            // shift denom one base digit to the right
            if (!denom.digits.empty()) denom.digits.erase(denom.digits.begin());
            denom.normalize();
        }
    }
    quotient.normalize();
    quotient.negative = result_negative && !quotient.is_zero();
    return quotient;
}

BigInt BigInt::operator%(const BigInt& other) const {
    if (other.is_zero()) throw std::runtime_error("Modulo by zero");
    BigInt div = (*this) / other; // uses operator/
    BigInt prod = div * other;
    BigInt rem = *this - prod;
    rem.negative = false;
    return rem;
}

BigInt BigInt::pow_mod(const BigInt& exp, const BigInt& mod) const {
    BigInt result(1);
    BigInt base = *this % mod;
    BigInt e = exp;
    while (!e.is_zero()) {
        if (e.digits[0] & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        long long carry = 0;
        for (int i = e.digits.size() - 1; i >= 0; --i) {
            long long cur = e.digits[i] + carry * 1LL * BASE;
            e.digits[i] = cur / 2;
            carry = cur % 2;
        }
        e.normalize();
    }
    return result;
}

std::string BigInt::to_hex() const {
    if (is_zero()) return "0";
    BigInt n = *this;
    n.negative = false;
    const char* hex_digits = "0123456789ABCDEF";
    std::string hex;
    while (!n.is_zero()) {
        BigInt rem = n % 16;
        int idx = rem.get_digit(0);
        hex = hex_digits[idx] + hex;
        n = n / 16;
    }
    return hex;
}

std::string BigInt::to_text() const {
    std::string hex = to_hex();
    if (hex.length() % 2) hex = "0" + hex;
    std::string text;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byte_str = hex.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byte_str, nullptr, 16));
        text += byte;
    }
    while (!text.empty() && text.back() == '\0') text.pop_back();
    return text;
}

BigInt BigInt::from_text(const std::string& text) {
    BigInt result(0);
    for (unsigned char c : text) result = result * 256 + BigInt((long long)c);
    return result;
}

std::ostream& operator<<(std::ostream& out, const BigInt& n) {
    if (n.negative) out << '-';
    out << n.digits.back();
    for (int i = (int)n.digits.size() - 2; i >= 0; --i) {
        out.width(BigInt::BASE_DIGITS);
        out.fill('0');
        out << n.digits[i];
    }
    return out;
}

bool BigInt::operator&(int bit) const {
    if (bit != 1) return false;
    if (digits.empty()) return false;
    return (digits[0] & 1) != 0;
}

void BigInt::operator>>=(int shift) {
    if (shift != 1) return;
    long long carry = 0;
    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        long long cur = digits[i] + carry * 1LL * BASE;
        digits[i] = (int)(cur / 2);
        carry = cur % 2;
    }
    normalize();
}

std::istream& operator>>(std::istream& in, BigInt& n) {
    std::string s;
    in >> s;
    n = BigInt(s);
    return in;
}
