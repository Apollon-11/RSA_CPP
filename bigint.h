#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <string>
#include <vector>

class BigInt {
private:
    static const int BASE = 1000000000;
    static const int BASE_DIGITS = 9;

    std::vector<int> digits;
    bool negative;

    void normalize();

public:
    BigInt();
    BigInt(long long n);
    BigInt(const std::string& s);

    bool is_zero() const;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;

    bool operator==(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator&(int bit) const;  
    void operator>>=(int shift);    

    BigInt pow_mod(const BigInt& exp, const BigInt& mod) const;

    std::string to_hex() const;
    std::string to_text() const;
    static BigInt from_text(const std::string& text);

    friend std::ostream& operator<<(std::ostream& out, const BigInt& n);
    friend std::istream& operator>>(std::istream& in, BigInt& n);
    int get_digit(int index) const {
        if (index >= 0 && index < (int)digits.size())
            return digits[index];
        return 0;
    }
};

#endif
