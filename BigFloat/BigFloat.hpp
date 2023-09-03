#pragma once

#include "../BigInt/BigInt.hpp"
#include <sstream>

// Either holds pointer to object of type T, or holds and owns a T object
template<typename T>
class ConstObjectOrPointer {
    std::variant<const T, const T*> _object;
public:
    ConstObjectOrPointer(T object) : _object(std::move(object)) {}
    ConstObjectOrPointer(const T* object) : _object(object) {}
    const T* operator->() const {
        if (std::holds_alternative<const T>(_object)) {
            return &std::get<const T>(_object);
        } else {
            return std::get<const T*>(_object);
        }
    }
};

// Simple BigFloat class that only supports positive numbers
class UnsignedBigFloat {
    UnsignedBigInt _mantissa;
    int64_t _exponent;

    // Precision of results
    static int64_t _precision;

    // Create a copy using the specified exponent if it differs from current exponent
    ConstObjectOrPointer<UnsignedBigFloat> usingExponent(int64_t exponent) const {
        if (exponent == _exponent) {
            return ConstObjectOrPointer<UnsignedBigFloat>(this);
        }

        auto copy = *this;
        copy._mantissa.timesTenToThe(exponent - _exponent);
        copy._exponent = exponent;
        return ConstObjectOrPointer(std::move(copy));
    }

    UnsignedBigFloat(UnsignedBigInt mantissa, int64_t exponent) : _mantissa(std::move(mantissa)), _exponent(exponent) {}
public:
    UnsignedBigFloat(std::string_view value) : _mantissa("0"), _exponent(0) {
        // Find location of the decimal point (If any)
        auto decimalPointIt = std::find(value.begin(), value.end(), '.');

        // If not found; no exponent
        if (decimalPointIt == value.end()) {
            _mantissa = UnsignedBigInt(value);
            _exponent = 0;
            return;
        }

        // Set the exponent based on the position of the decimal point
        _exponent = std::distance(decimalPointIt, std::prev(value.end()));

        // Otherwise copy all digits into a string and use it as mantissa (skip the '.')
        UnsignedBigInt::DigitString copy;
        copy.reserve(value.size()-1);
        auto charToInt = [](auto c) { return c - '0'; };
        std::transform(value.begin(), decimalPointIt, std::back_inserter(copy), charToInt);
        std::transform(std::next(decimalPointIt), value.end(), std::back_inserter(copy), charToInt);
        _mantissa = UnsignedBigInt::fromIntString(std::move(copy));
    }

    // Arithmetic Operators
    UnsignedBigFloat operator+(const UnsignedBigFloat& other) const {
        const auto exponent = std::max(_exponent, other._exponent);
        const auto one = usingExponent(exponent);
        const auto two = other.usingExponent(exponent);
        return UnsignedBigFloat(one->_mantissa + two->_mantissa, exponent);
    }
    UnsignedBigFloat operator-(const UnsignedBigFloat& other) const {
        const auto exponent = std::max(_exponent, other._exponent);
        const auto one = usingExponent(exponent);
        const auto two = other.usingExponent(exponent);
        return UnsignedBigFloat(one->_mantissa - two->_mantissa, exponent);
    }
    UnsignedBigFloat operator*(const UnsignedBigFloat& other) const {
        return UnsignedBigFloat(_mantissa * other._mantissa, _exponent + other._exponent);
    }
    UnsignedBigFloat operator/(const UnsignedBigFloat& other) const {
        const auto a = usingExponent(other._exponent + _precision);
        return UnsignedBigFloat(a->_mantissa / other._mantissa, a->_exponent - other._exponent);
    }

    // Comparison Operators
    bool operator==(const UnsignedBigFloat& other) const {
        const auto exponent = std::max(_exponent, other._exponent);
        const auto one = usingExponent(exponent);
        const auto two = other.usingExponent(exponent);
        return one->_mantissa == two->_mantissa;
    }
    bool operator!=(const UnsignedBigFloat& other) const { return !(*this == other); }
    bool operator<(const UnsignedBigFloat& other) const {
        const auto exponent = std::max(_exponent, other._exponent);
        const auto one = usingExponent(exponent);
        const auto two = other.usingExponent(exponent);
        return one->_mantissa < two->_mantissa;
    }
    bool operator>(const UnsignedBigFloat& other) const {
        const auto exponent = std::max(_exponent, other._exponent);
        const auto one = usingExponent(exponent);
        const auto two = other.usingExponent(exponent);
        return one->_mantissa > two->_mantissa;
    }

    // Stream Out Operator
    friend std::ostream& operator<<(std::ostream& os, const UnsignedBigFloat& f) {
        std::stringstream ss;
        ss << f._mantissa;
        auto str = ss.str();

        if (str.length() < size_t(f._exponent)) {
            const auto padding = std::string(std::abs(f._exponent) - str.length(), '0');
            str = padding + str;
        }
        str.insert(str.end() - f._exponent, '.');
        while (str.back() == '0') {
            str.pop_back();
        }
        if (str.back() == '.') {
            str.push_back('0');
        }

        os << str;
        return os;
    }

    static void setGlobalPrecision(int64_t precision) {
        assert(precision > 0);
        _precision = precision;
    }

    // The smallest difference possible between two numbers
    static auto epsilon() {
        return UnsignedBigFloat(UnsignedBigInt("1"), _precision-1);
    }
};

int64_t UnsignedBigFloat::_precision = 10;

// Square root function implemented using binary search til in-precision
// is smaller than our current global setting allows
auto sqrt(UnsignedBigFloat x) {
    auto left = UnsignedBigFloat("0.0");
    auto right = x;
    const auto two = UnsignedBigFloat("2.0");
    const auto epsilon = UnsignedBigFloat::epsilon();

    while ((right - left) > epsilon) {
        const auto mid = (left + right) / two;
        if ((mid * mid) > x) {
            right = mid;
        } else {
            left = mid;
        }
    }

    return left;
}