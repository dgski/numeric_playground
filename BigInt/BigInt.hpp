#pragma once

#include <string_view>
#include <iostream>
#include <algorithm>

// Simple implementation of theoretically unlimited-width integers
class BigInt {
    friend std::ostream& operator<<(std::ostream& os, const BigInt& value);

    bool _negative = false;
    std::string _digits;
    BigInt() {}

    void multiplyByTen() {
        _digits = char(0) + _digits;
    }

    void divideByTen() {
        _digits.erase(_digits.begin());
    }

    BigInt oppositelySigned() const {
        auto copy = *this;
        copy._negative = !copy._negative;
        return copy;
    }

    BigInt absoluteValue() const {
        auto copy = *this;
        copy._negative = false;
        return copy;
    }

    static int consumeFrontDigit(std::string_view& value) {
        if (value.empty()) {
            return 0;
        }
        const auto result = value.front();
        value.remove_prefix(1);
        return int(result);
    };
public:
    BigInt(std::string_view value) {
        if (value.front() == '-') {
            _negative = true;
            value.remove_prefix(1);
        }
        _digits.reserve(value.size());
        for (char c : value) {
            _digits.push_back(c - '0');
        }
        std::reverse(_digits.begin(), _digits.end());
    }

    BigInt(int64_t value) : BigInt(std::to_string(value)) {}

    BigInt operator+(const BigInt& other) const {
        if (_negative && !other._negative) {
            return other - (*this).oppositelySigned();
        }
        if (!_negative && other._negative) {
            return (*this) - other.oppositelySigned();
        }

        BigInt result;
        result._negative = _negative;
        int carry = 0;

        std::string_view one(_digits), two(other._digits);
        while (!one.empty() || !two.empty()) {
            const int nextDigit = consumeFrontDigit(one) + consumeFrontDigit(two) + carry;
            if (nextDigit <= 9) {
                result._digits.push_back(nextDigit);
                carry = 0;
            } else {
                result._digits.push_back(nextDigit % 10);
                carry = 1;
            }
        }
        if (carry) {
            result._digits.push_back(carry);
        }

        return result;
    }

    BigInt operator-(const BigInt& other) const {
        if (*this == other) {
            return BigInt("0");
        }

        const bool signFlip =
            (_negative == other._negative) &&
            ((_negative && (*this > other)) || (!_negative && (*this < other)));
        if (signFlip) {
            return (other - *this).oppositelySigned();
        }

        // Subtracting Negative From Positive or Subtracting Positive From Negative
        if (_negative != other._negative) {
            return (*this) + other.oppositelySigned();
        }

        // Subtracting smaller number of same sign:
        BigInt result;
        result._negative = _negative;
        int borrow = 0;

        std::string_view one(_digits), two(other._digits);
        while (!one.empty() || !two.empty()) {
            int nextDigit = consumeFrontDigit(one) - consumeFrontDigit(two) - borrow;
            if (nextDigit < 0) {
                nextDigit += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result._digits.push_back(nextDigit);
        }

        while (result._digits.back() == 0) {
            result._digits.pop_back();
        }

        return result;
    }

    BigInt operator*(const BigInt& other) const {
        if (*this == BigInt("0") || other == BigInt("0") || _digits.empty() || other._digits.empty()) {
            return BigInt("0");
        }

        if (*this == BigInt("1")) {
            return other;
        }

        if (other == BigInt("1")) {
            return *this;
        }

        if (_negative && other._negative) {
            return oppositelySigned() * other.oppositelySigned();
        }
        if (_negative && !other._negative) {
            auto result = oppositelySigned() * other;
            result._negative = true;
            return result;
        }
        if (!_negative && other._negative) {
            auto result = (*this) * other.oppositelySigned();
            result._negative = true;
            return result;
        }

        // Multiple other by first digit
        auto currentDigitResult = BigInt("0");
        const auto currentDigit = _digits.front();
        auto multiplier = BigInt("1");
        for (auto digit : other._digits) {
            currentDigitResult = currentDigitResult + (multiplier * (BigInt(currentDigit * digit)));
            multiplier.multiplyByTen();
        }

        // Add result and recurse
        auto next = *this;
        next._digits.erase(next._digits.begin());

        auto nextValue = (next * other);
        nextValue.multiplyByTen();

        return currentDigitResult + nextValue;
    }

    BigInt operator/(const BigInt& other) const {
        if (other == BigInt("0")) {
            throw std::runtime_error("Dividing by zero");
        }
        if (*this == BigInt("0")) {
            return BigInt("0");
        }

        if (_negative && other._negative) {
            return oppositelySigned() / other.oppositelySigned();
        }
        if (_negative != other._negative) {
            auto result = absoluteValue() / other.absoluteValue();
            result._negative = true;
            return result;
        }

        auto multiplier = BigInt("1");
        while ((multiplier * other) < (*this)) {
            multiplier.multiplyByTen();
        }
        multiplier.divideByTen();

        auto result = BigInt("0");
        auto remaining = *this;
        remaining._negative = false;
        while (remaining >= other) {
            result = result + multiplier;
            remaining = remaining - (other * multiplier);
        }

        if (!(_negative && other._negative) && (_negative || other._negative)) {
            result._negative = true;
        }

        if ((remaining < other) || multiplier == BigInt("1")) {
            return result;
        }

        return remaining / other;
    }

    bool operator==(const BigInt& other) const {
        return std::tie(_negative, _digits) == std::tie(other._negative, other._digits);
    }

    bool operator<(const BigInt& other) const {
        if (_negative && !other._negative) {
            return true;
        }
        if (!_negative && other._negative) {
            return false;
        }
        if (_digits.size() == other._digits.size()) {
            if (_negative) {
                return _digits.back() > other._digits.back();
            } else {
                return _digits.back() < other._digits.back();
            }
        }

        if (_negative) {
            return _digits.size() > other._digits.size();
        } else {
            return _digits.size() < other._digits.size();
        }
    }

    bool operator>(const BigInt& other) const {
        return (*this != other) && !(*this < other);
    }

    bool operator<=(const BigInt& other) const {
        return (*this == other) || (*this < other);
    }

    bool operator>=(const BigInt& other) const {
        return (*this == other) || (*this > other);
    }
};

std::ostream& operator<<(std::ostream& os, const BigInt& value) {
    if (value._negative) {
        os << '-';
    }
    for (int i=value._digits.size()-1; i>=0; --i) {
        os << int(value._digits[i]);
    }
    return os;
}