#pragma once

#include "../BigInt/BigInt.hpp"
#include <sstream>

class BigFloat {
    friend std::ostream& operator<<(std::ostream& os, const BigFloat&);

    BigInt _mantissa;
    int64_t _exponent;

    static int64_t _precision;

    BigFloat() : _mantissa("0"), _exponent(0) {}

    BigFloat usingExponent(int64_t exponent) const {
        auto copy = *this;
        return copy.useExponent(exponent);
    }

    BigFloat& useExponent(int64_t exponent) {
        if (_exponent == exponent) {
            return *this;
        } else if (_exponent < exponent) {
            _mantissa.timesTenToThe(exponent - _exponent);
            _exponent = exponent;
            return *this;
        } else /* copy._exponent > exponent */ {
            _mantissa.timesTenToThe(_exponent - exponent);
            _exponent = exponent;
            return *this;
        }

        return *this;
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
        auto copy = *this;
        copy += other;
        return copy;
    }
    BigFloat& operator+=(const BigFloat& other) {
        if (_exponent != other._exponent) {
            const auto exponent = std::min(_exponent, other._exponent);
            useExponent(exponent) += other.usingExponent(exponent);
            return *this;
        }
        _mantissa += other._mantissa;
        return *this;
    }

    BigFloat operator-(const BigFloat& other) const {
        auto copy = *this;
        copy -= other;
        return copy;
    }
    BigFloat& operator-=(const BigFloat& other) {
        if (_exponent != other._exponent) {
            const auto exponent = std::min(_exponent, other._exponent);
            useExponent(exponent) -= other.usingExponent(exponent);
            return *this;
        }
        _mantissa -= other._mantissa;
        return *this;
    }

    BigFloat operator*(const BigFloat& other) const {
        auto copy = *this;
        copy *= other;
        return copy;
    }
    BigFloat& operator*=(const BigFloat& other) {
        _mantissa *= other._mantissa;
        _exponent += other._exponent;
        return *this;
    }

    BigFloat operator/(const BigFloat& other) const {
        auto copy = *this;
        copy /= other;
        return copy;
    }
    BigFloat& operator/=(const BigFloat& other) {
        useExponent(other._exponent - _precision);
        _mantissa /= other._mantissa;
        _exponent -= other._exponent;
        return *this;
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

    static void setGlobalPrecision(int64_t settingPrecision) {
        _precision = settingPrecision;
    }

    static BigFloat epsilon() {
        std::string str = "0." + std::string(_precision-1, '0') + '1';
        return BigFloat(str);
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

int64_t BigFloat::_precision = 100;