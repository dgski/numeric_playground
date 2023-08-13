#pragma once

#include "../BigInt/BigInt.hpp"
#include <sstream>

class BigFloat {
    friend std::ostream& operator<<(std::ostream& os, const BigFloat&);

    BigInt _mantissa;
    int64_t _exponent;

    static const int64_t defaultPrecisionExponent = 10;

    BigFloat() : _mantissa("0"), _exponent(0) {}

    BigFloat usingExponent(int64_t exponent) const {
        auto copy = *this;

        if (copy._exponent == exponent) {
            return copy;
        } else if (copy._exponent < exponent) {
            copy._mantissa = copy._mantissa * BigInt(std::pow(10, (exponent - copy._exponent)));
            copy._exponent = exponent;
            return copy;
        } else /* copy._exponent > exponent */{
            copy._mantissa = copy._mantissa * BigInt(std::pow(10, (copy._exponent - exponent)));
            copy._exponent = exponent;
            return copy;
        }
    }
public:
    BigFloat(std::string_view value) : _mantissa("0"), _exponent(0) {
        auto copy = std::string(value.begin(), value.end());
        auto it = std::find(copy.begin(), copy.end(), '.');
        _exponent = std::distance(copy.end(), it) + 1;
        copy.erase(it);
        _mantissa = BigInt(copy);
    }

    BigFloat operator+(const BigFloat& other) const {
        if (_exponent != other._exponent) {
            const auto exponent = std::min(_exponent, other._exponent);
            return usingExponent(exponent) + other.usingExponent(exponent);
        }

        BigFloat result;
        result._mantissa = _mantissa + other._mantissa;
        result._exponent = _exponent;
        return result;
    }

    BigFloat operator-(const BigFloat& other) const {
        if (_exponent != other._exponent) {
            const auto exponent = std::min(_exponent, other._exponent);
            return usingExponent(exponent) - other.usingExponent(exponent);
        }

        BigFloat result;
        result._mantissa = _mantissa - other._mantissa;
        result._exponent = _exponent;
        return result;
    }

    BigFloat operator*(const BigFloat& other) const {
        BigFloat result;
        result._mantissa = _mantissa * other._mantissa;
        result._exponent = _exponent + other._exponent;
        return result;
    }

    BigFloat operator/(const BigFloat& other) const {
        BigFloat result;
        result._mantissa = _mantissa / other._mantissa;
        std::cout << "newMantissa=" << result._mantissa << std::endl;
        result._exponent = _exponent + other._exponent;
        return result;
    }

    bool operator==(const BigFloat& other) const {
        // TODO: do not use strings
        std::ostringstream one, two;
        one << (*this);
        two << other;
        return one.str() == two.str();
    }

    bool operator!=(const BigFloat& other) const {
        return !(*this == other);
    }
};

std::ostream& operator<<(std::ostream& os, const BigFloat& value)
{
    std::ostringstream ss;
    ss << value._mantissa;
    auto str = ss.str();
    str.insert(str.end() + value._exponent, '.');
    os << str;
    return os;
}