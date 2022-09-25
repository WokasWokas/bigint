#include "bigint.h"
#include <cstring>
#include <string>

unsigned long BigInt::length() const {
	if (value == "0")
		return 0;
	return value.length();
}

BigInt::BigInt() {
	value = "0";
}

BigInt::BigInt(const std::string &s) {
	unsigned long start = s.find_first_not_of("0-");

	if (!std::strcmp(&s[start], "")) {
		std::strcpy(&value[0], "0");
	}

	if (s[0] == '-') {
		sign = 1;
	}
	value = &s[start];
}

BigInt::BigInt(long int a)
{
	sign = 0;
	if (a < 0) {
		sign = 1;
		a *= -1;
	}
	value = std::to_string(a);
}

BigInt BigInt::operator=(long int a) {
	return *new BigInt(a);
}

std::string BigInt::getValue() const {
	return value;
}

bool BigInt::getSign() const {
	return sign;
}

void BigInt::setSign(const bool s) {
	sign = s;
}

BigInt::operator int() const {
	int final = 0;
	int pow = 1;
	for (long i = length() - 1; i >= 0; i--) {
		final += pow * (value[i] - '0');
		pow *= 10;
	}
	if (sign)
		final *= -1;
	return final;
}

std::string BigInt::getBinary() const {
	if (!length())
		return "0";
	BigInt temp = *new BigInt(value);

	std::string result = "";
	result = result + (char)((int)sign + '0');
	std::string current;
	 
	while (temp > 0) {
		current = (temp % 2).getValue();
		if (current.length() == 0)
			result = result + "0";
		else
			result = result + current;
		temp = temp / 2;
	}

	return result;
}

void BigInt::initFromBinary(const std::string b) {
	sign = b[0] - '0';
	BigInt temp = 0;
	BigInt shift = 1;
	for (long i = 1; i < b.length(); i++) {
		temp += *new BigInt((int)(b[i] - '0')) * shift;
		shift = shift * 2;
	}
	value = temp.value;
}

std::ostream& operator<<(std::ostream& os, const BigInt& a) {
	if (a.length() == 0 || a.getValue()[0] == '0') {
		std::string zero = "0";
		return os << zero;
	}
	if (a.getSign())
		return os << "-" + a.getValue();
	return os << a.getValue();
}

std::string BigInt::align(const unsigned long l) const {
	std::string newStr = value;
	for (unsigned long i = 0; i < l; i++) 
		newStr = "0" + newStr;
	return newStr;
}

BigInt BigInt::alignLeft(const unsigned long l) const {
	std::string newStr = value;
	for (unsigned long i = 0; i < l; i++)
		newStr = newStr + "0";
	return newStr;
}

void BigInt::operator++() {
	*this += 1;
}

void BigInt::operator--() {
	*this = *this - 1;
}

std::string BigInt::add(const BigInt& a) const {
	unsigned long numOfZeros = value.length() - a.length();
	std::string aligned = a.align(numOfZeros);
	std::string result;
	int x = 0;
	bool carry = 0;
	for (long i = value.length() - 1; i >= 0; i--) {
		x = value[i] + aligned[i] - 96 + carry;
		carry = x / 10;
		result = (char)(x % 10 + 48) + result;
	}
	if (carry)
		result = "1" + result;
	return result;
}

BigInt BigInt::operator+(const BigInt& a) const {
	if (!getSign() && a.getSign()) {
		BigInt* temp = new BigInt(a.value);
		return *this - *temp;
	}
	if (getSign() && !a.getSign()) {
		BigInt* temp = new BigInt(value);
		return a - *temp;
	}
	if (value.length() < a.length())
		return a.operator+(*this);
	std::string result = this->add(a);
	if (getSign() && a.getSign())
		return *new BigInt('-' + result);
	return *new BigInt(result);
}

std::string BigInt::subtract(const BigInt& a) const {
	if (a.length() > length())
		return a.subtract(*this);
	long numOfZeros = this->length() - a.length();
	std::string aligned = a.align(numOfZeros);
	std::string result = "";
	int x = 0;
	bool carry = 0;
	for (long i = value.length() - 1; i >= 0; i--) {
		x = value[i] - aligned[i] - carry;
		carry = false;
		if (x < 0) {
			x += 10;
			carry = true;
		}
		result = (char)(x + 48) + result;
	}
	return result;
}

BigInt BigInt::operator-(const BigInt& a) const {
	if (length() == 0)
		return a * *new BigInt(-1);
	if (a.length() == 0)
		return *this * *new BigInt(-1);
	if (!getSign() && !a.getSign()) {
		if (*this > a)
			return *new BigInt(this->subtract(a));
		BigInt* result = new BigInt(a.subtract(*this));
		result->setSign(true);
		return *result;
	}
	if (!getSign() && a.getSign())
		return *new BigInt(this->add(a));
	if (getSign() && !a.getSign()) {
		BigInt result = this->add(a);
		result.setSign(true);
		return result;
	}
	BigInt temp1 = *new BigInt(value);
	BigInt temp2 = *new BigInt(a.value);
	return temp2 - temp1;
}

void BigInt::operator+=(const BigInt& a) {
	BigInt temp = *this + a;
	value = temp.value;
	sign = temp.sign;
}

void BigInt::operator-=(const BigInt& a) {
	BigInt temp = *this - a;
	value = temp.value;
	sign = temp.sign;
}

BigInt BigInt::operator*(const BigInt& a) const {
	BigInt final = 0;
	std::string result;
	BigInt* temp;
	int carry;
	int current;
	for (long i = length() - 1; i >= 0; i--) {
		carry = 0;
		result = "";
		for (long j = a.length() - 1; j >= 0; j--) {
			current = (value[i] - '0') * (a.value[j] - '0') + carry;
			result = (char)(current % 10 + '0') + result;
			carry = current / 10;
		}
		if (carry > 0)
			result = (char)(carry + '0') + result;
		temp = new BigInt(result);
		final += *new BigInt(temp->alignLeft(length() - i - 1));
	}
	final.setSign(sign ^ a.sign);
	return final;
}

BigInt BigInt::operator/(const BigInt& a) const {
	if (a.length() == 0 || a.getValue()[0] == '0')
		throw "Devision By Zero";
	BigInt divider = *new BigInt(a.getValue());
	if (divider > *new BigInt(value))
		return *new BigInt();
	std::string result;
	int idx = 0;
	BigInt temp = value[idx] - '0';
	while (temp < divider.value) 
		temp = temp * 10 + (value[++idx] - '0');
	while (temp == 0) {
		if (temp == 0) {
			result = result + "0";
			idx++;
		}
		else {
			BigInt multNum = 1;
			BigInt leftover = temp - divider;
			while (leftover >= divider) {
				leftover -= divider;
				multNum += 1;
			}
			leftover = temp - (multNum * divider);
			result = result + multNum.getValue();
			temp = leftover * 10 + (value[++idx] - '0');
			temp.setSign(false);
		}
	}
	if (result.length() == 0)
		return *new BigInt();

	BigInt final = *new BigInt(result);
	final.setSign(this->sign ^ a.getSign());
	return final;
}

BigInt BigInt::operator%(const BigInt& a) const {
	if (a.length() == 0 || a.getValue()[0] == '0')
		throw "Modulo By Zero";
	if (a > *this)
		return *new BigInt(*this);
	BigInt divider = *new BigInt(a.getValue());
	if (divider > *new BigInt(value))
		return (*this + a) & a;
	std::string result;
	int idx = 0;
	BigInt leftover;
	BigInt temp = value[idx] - '0';
	while (temp < divider.value)
		temp = temp * 10 + (value[++idx] - '0');
	while (idx < length()) {
		BigInt multNum = 0;
		leftover = temp;
		while (leftover >= divider) {
			leftover -= divider;
			multNum += 1;
		}
		leftover = temp - (multNum * divider);
		leftover.setSign(false);
		temp = leftover * 10 + (value[++idx] - '0');
	}
	return leftover;
}

bool BigInt::operator<(const BigInt& a) const {
	if (getSign() && !a.getSign())
		return true;
	if (!getSign() && a.getSign())
		return false;
	unsigned long l1 = length(), l2 = a.length();
	if (!getSign() && !a.getSign()) {
		if (l1 > l2)
			return false;
		if (l1 < l2)
			return true;
		for (long i = 0; i < l1; i++) {
			if (value[i] > a.value[i])
				return false;
			if (value[i] < a.value[i])
				return true;
		}
	}
	else {
		if (l1 > l2)
			return true;
		if (l1 < l2)
			return false;
		for (long i = 0; i < l1; i++) {
			if (value[i] > a.value[i])
				return true;
			if (value[i] < a.value[i])
				return false;
		}
	}
	return false;
}

bool BigInt::operator<=(const BigInt& a) const {
	if (getSign() && !a.getSign())
		return true;
	if (!getSign() && a.getSign())
		return false;
	unsigned long l1 = length(), l2 = a.length();
	if (!getSign() && !a.getSign()) {
		if (l1 > l2)
			return false;
		if (l1 < l2)
			return true;
		for (long i = 0; i < l1; i++) {
			if (value[i] > a.value[i])
				return false;
			if (value[i] < a.value[i])
				return true;
		}
	}
	else {
		if (l1 > l2)
			return true;
		if (l1 < l2)
			return false;
		for (long i = 0; i < l1; i++) {
			if (value[i] > a.value[i])
				return true;
			if (value[i] < a.value[i])
				return false;
		}
	}
	return true;
}

bool BigInt::operator>(const BigInt& a) const {
	if (getSign() && !a.getSign())
		return false;
	if (!getSign() && a.getSign())
		return true;
	unsigned long l1 = length(), l2 = a.length();
	if (getSign() && a.getSign()) {
		if (l1 > l2)
			return false;
		if (l1 < l2)
			return true;
		for (long i = 0; i < l1; i++) {
			if (value[i] > a.value[i])
				return false;
			if (value[i] < a.value[i])
				return true;
		}
	}
	else {
		if (l1 > l2)
			return true;
		if (l1 < l2)
			return false;
		for (long i = 0; i < l1; i++) {
			if (value[i] > a.value[i])
				return true;
			if (value[i] < a.value[i])
				return false;
		}
	}
	return false;
}

bool BigInt::operator>=(const BigInt& a) const {
	if (getSign() && !a.getSign())
		return false;
	if (!getSign() && a.getSign())
		return true;
	unsigned long l1 = length(), l2 = a.length();
	if (getSign() && a.getSign()) {
		if (l1 > l2)
			return false;
		if (l1 < l2)
			return true;
		for (long i = 0; i < l1; i++) {
			if (value[i] > a.value[i])
				return false;
			if (value[i] < a.value[i])
				return true;
		}
	}
	else {
		if (l1 > l2)
			return true;
		if (l1 < l2)
			return false;
		for (long i = 0; i < l1; i++) {
			if (value[i] > a.value[i])
				return true;
			if (value[i] < a.value[i])
				return false;
		}
	}
	return true;
}

bool BigInt::operator==(const BigInt& a) const {
	if (length() == 0 && a.length() == 0)
		return true;
	if (getSign() ^ a.getSign())
		return false;
	if (length() != a.length())
		return false;
	for (long i = 0; i < length(); i++) {
		if (value[i] != a.value[i])
			return false;
	}
	return true;
}

bool BigInt::operator!=(const BigInt& a) const {
	return !(*this == a);
}

void BigInt::operator<<=(const BigInt& a) {
	BigInt result = *this << a;
	value = result.getValue();
	sign = result.getSign();
}

void BigInt::operator>>=(const BigInt& a) {
	BigInt result = *this >> a;
	value = result.getValue();
	sign = result.getSign();
}

void BigInt::operator&=(const BigInt& a) {
	BigInt result = *this & a;
	value = result.getValue();
	sign = result.getSign();
}

BigInt BigInt::operator^(const BigInt& a) const {
	std::string b1 = this->getBinary();
	std::string b2 = a.getBinary();
	long l1 = b1.length(), l2 = b2.length();
	int numOfZeros;
	if (l1 > l2) {
		numOfZeros = l1 - l2;
		for (int i = 0; i < numOfZeros; i++)
			b2 = b2 + "0";
	}
	else if (l2 > l1) {
		numOfZeros = l2 - l1;
		for (int i = 0; i < numOfZeros; i++)
			b1 = b1 + "0";
	}
	std::string result = "";
	for (int i = 0; i < l1 + 1; i++) {
		if (b1[i] == '1' ^ b2[i] == '1')
			result = result + "1";
		else
			result = result + "0";
	}
	BigInt final = *new BigInt();
	final.initFromBinary(result);
	return final;
}

BigInt BigInt::operator|(const BigInt& a) const {
	std::string b1 = this->getBinary();
	std::string b2 = a.getBinary();
	long l1 = b1.length();
	long l2 = b2.length();
	int numOfZeros;
	if (l1 > l2) {
		numOfZeros = l1 - l2;
		for (int i = 0; i < numOfZeros; i++)
			b2 = b2 + "0";
	}
	else if (l2 > l1) {
		numOfZeros = l2 - l1;
		for (int i = 0; i < numOfZeros; i++)
			b1 = b1 + "0";
	}
	std::string result = "";
	for (int i = 0; i < l1; i++) {
		if (b1[i] == '1' || b2[i] == '1')
			result = result + "1";
		else
			result = result + "0";
	}
	BigInt final = *new BigInt();
	final.initFromBinary(result);
	return final;
}

BigInt BigInt::operator&(const BigInt& a) const {
	std::string b1 = this->getBinary();
	std::string b2 = a.getBinary();
	long l1 = b1.length();
	long l2 = b2.length();
	int numOfZeros;
	if (l1 > l2) {
		numOfZeros = l1 - l2;
		for (int i = 0; i < numOfZeros; i++)
			b2 = b2 + "0";
	}
	else if (l2 > l1) {
		numOfZeros = l2 - l1;
		for (int i = 0; i < numOfZeros; i++)
			b1 = b1 + "0";
	}
	std::string result = "";
	for (int i = 0; i < l1; i++) {
		if (b1[i] == '1' && b2[i] == '1')
			result = result + "1";
		else
			result = result + "0";
	}
	BigInt final = *new BigInt();
	final.initFromBinary(result);
	return final;
}

BigInt BigInt::operator<<(const BigInt& a) const {
	std::string b = this->getBinary();
	std::string result = "";
	result = result + b[0];
	for (BigInt i = 0; i < a; i += 1) {
		result = result + "0";
	}
	for (long j = 1; j < b.length(); j++) {
		result = result + b[j];
	}
	BigInt final = *new BigInt();
	final.initFromBinary(result);
	return final;
}

BigInt BigInt::operator>>(const BigInt& a) const {
	std::string b = this->getBinary();
	b.erase(1, (int)a);
	BigInt final = *new BigInt();
	final.initFromBinary(b);
	return final;
}

bool BigInt::isPrime() {
	if (*this == 2 || *this == 3)
		return true;
	if (*this % 2 == 0 || *this % 3 == 0 || *this == 1)
		return false;
	for (BigInt i = 5; i * i < *this; i += 4)
		if (*this % i == 0 || *this % (i + 2) == 0)
			return false;
	return true;
}