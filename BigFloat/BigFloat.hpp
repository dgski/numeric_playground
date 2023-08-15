#pragma once

#include "../BigInt/BigInt.hpp"
#include <sstream>

class BigFloat {
    friend std::ostream& operator<<(std::ostream& os, const BigFloat&);

    BigInt _mantissa;
    int64_t _exponent;

    BigFloat() : _mantissa("0"), _exponent(0) {}

    BigFloat usingExponent(int64_t exponent) const {
        auto copy = *this;

        if (copy._exponent == exponent) {
            return copy;
        } else if (copy._exponent < exponent) {
            copy._mantissa.timesTenToThe(exponent - copy._exponent);
            copy._exponent = exponent;
            return copy;
        } else /* copy._exponent > exponent */ {
            copy._mantissa.timesTenToThe(copy._exponent - exponent);
            copy._exponent = exponent;
            return copy;
        }
    }

    BigFloat abs() const {
        auto result = *this;
        result._mantissa = result._mantissa.absoluteValue();
        return result;
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
        auto one = usingExponent(-50); // TODO: figure out precision
        auto two = other.usingExponent(-10); // TODO: figure out precision
        BigFloat result;
        result._mantissa = one._mantissa / two._mantissa;
        result._exponent = one._exponent - two._exponent;
        return result.usingExponent(-50);
    }

    bool operator==(const BigFloat& other) const {
        const auto exponent = std::min(_exponent, other._exponent);
        auto one = usingExponent(exponent);
        auto two = other.usingExponent(exponent);
        return one._mantissa == two._mantissa;
    }

    bool operator!=(const BigFloat& other) const {
        return !(*this == other);
    }

    bool operator<(const BigFloat& other) const {
        const auto exponent = std::min(_exponent, other._exponent);
        auto one = usingExponent(exponent);
        auto two = other.usingExponent(exponent);
        return one._mantissa < two._mantissa;
    }

    bool operator>(const BigFloat& other) const {
        const auto exponent = std::min(_exponent, other._exponent);
        auto one = usingExponent(exponent);
        auto two = other.usingExponent(exponent);
        return one._mantissa > two._mantissa;
    }

    void dump() const {
        std::cout << " mantissa=" << _mantissa << " exponent=" << _exponent << std::endl;
    }
};

std::ostream& operator<<(std::ostream& os, const BigFloat& value)
{
    if (value._mantissa == BigInt("0")) {
        os << "0.0";
        return os;
    }

    std::ostringstream ss;
    ss << value._mantissa.absoluteValue();
    auto str = ss.str();

    if (value._exponent == 0) {
        str += ".0";
    } else if (value._exponent < 0) {
        if (str.length() < size_t(std::abs(value._exponent))) {
            const auto padding = std::string(std::abs(value._exponent) - str.length(), '0');
            str = padding + str;
        }
        str.insert(str.end() + value._exponent, '.');
    } else if (value._exponent > 0) {
        for (auto i=0; i<value._exponent; ++i) {
            str.push_back('0');
        }
        str += ".0";
    }

    os << ((value._mantissa >= BigInt("0")) ? "" : "-") << str;
    return os;
}