#include <string>
#ifndef BIGINT_H
#define BIGINT_H
class BigInt {
    std::string value;
    bool sign = 0;
public:
    BigInt();
    BigInt(const std::string&);
    BigInt(long int);
    BigInt operator=(long int);
    explicit operator int() const;
    std::string getValue() const;
    bool getSign() const;
    bool isPrime();
    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    BigInt operator*(const BigInt&) const;
    BigInt operator/(const BigInt&) const;
    BigInt operator%(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>=(const BigInt&) const;
    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt& a) const;
    void operator+=(const BigInt&);
    void operator-=(const BigInt&);
    void operator++();
    void operator--();
    BigInt operator&(const BigInt&) const;
    BigInt operator<<(const BigInt&) const;
    BigInt operator>>(const BigInt& a) const;
    void operator<<=(const BigInt&);
    void operator>>=(const BigInt&);
    void operator&=(const BigInt&);
    BigInt operator^(const BigInt&) const;
    BigInt operator|(const BigInt&) const;
    unsigned long length() const;
    std::string align(const unsigned long) const;
    void initFromBinary(const std::string);
private:
    std::string getBinary() const;
    std::string subtract(const BigInt&) const;
    std::string add(const BigInt&) const;
    void setSign(const bool);
    BigInt alignLeft(const unsigned long) const;
};
std::ostream& operator<<(std::ostream& os, const BigInt&);
#endif