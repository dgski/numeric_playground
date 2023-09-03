#pragma once

#include <string_view>
#include <iostream>
#include <algorithm>
#include <cassert>

// Simple BigInt class that only supports positive integers
class UnsignedBigInt {
public:
    using DigitType = int32_t;
    using DigitString = std::basic_string<DigitType>;
    using DigitStringView = std::basic_string_view<DigitType>;
private:
    static constexpr DigitType BASE = 10;

    // Digits of base-10 integer stored in human readable order
    DigitString _digits;

    // Helper function to consume the least significant digit
    static DigitType consumeNextDigit(DigitStringView& value) {
        if (value.empty()) {
            return 0;
        }
        const auto result = value.back();
        value.remove_suffix(1);
        return result;
    };

    // Division/Remainder operation implemented using iterative subtraction chunks
    // Each iteration sizes the chunk to be largest possible but still smaller than remainder
    static std::pair<UnsignedBigInt, UnsignedBigInt> divModChunked(
        const UnsignedBigInt& dividend,
        const UnsignedBigInt& divisor)
    {
        if (dividend < divisor) {
            return { UnsignedBigInt("0"), dividend };
        }

        auto quotient = UnsignedBigInt("0");
        auto remainder = dividend;
        auto multiplier = UnsignedBigInt("1");
        auto chunk = divisor;
        const auto initialChunkMultiplier = remainder._digits.size() - divisor._digits.size();
        multiplier.timesBaseToThe(initialChunkMultiplier);
        chunk.timesBaseToThe(initialChunkMultiplier);

        // Continue until remainder is smaller than divisor
        while (divisor <= remainder) {
            // Find next chunk size
            while (chunk > remainder) {
                multiplier.divideByTen();
                chunk.divideByTen();
            }

            // Find how many chunks fit into remainder and therefore how many 'divisors's
            while (remainder >= chunk) {
                remainder = remainder - chunk;
                quotient = quotient + multiplier;
            }
        }

        return { quotient, remainder };
    }

    // Division and remainder implemented using 'grade-school' long division
    static std::pair<UnsignedBigInt, UnsignedBigInt> divModLong(
        const UnsignedBigInt& dividend,
        const UnsignedBigInt& divisor)
    {
        if (dividend < divisor) {
            return { UnsignedBigInt("0"), dividend };
        }

        // Still digits left to process
        auto dividendDigitsAreLeft = [&](auto currentIndex) {
            return size_t(currentIndex) != dividend._digits.size();
        };

        // Put next reasonable segment to divide by divisor into remainder, update the currentIndex of the dividends digits
        // and return the number of digits in the next segment (front-padded zeros included)
        auto prepareNextDividendSegment = [&](auto& remainder, auto& currentIndex) {
            auto segmentWidth = 0;
            while (dividendDigitsAreLeft(currentIndex) && (remainder < divisor)) {
                if (remainder == UnsignedBigInt("0")) {
                    remainder._digits = { dividend._digits[currentIndex] };
                } else {
                    remainder._digits.push_back(dividend._digits[currentIndex]);
                }
                ++segmentWidth;
                ++currentIndex;
            }
            return segmentWidth;
        };

        auto quotientString = DigitString{};
        auto remainder = UnsignedBigInt("0");
        auto currentIndex = 0;
        while (dividendDigitsAreLeft(currentIndex)) {
            const auto segmentWidth = prepareNextDividendSegment(remainder, currentIndex);
            const auto [segmentQuotient, segmentRemainder] = divModChunked(remainder, divisor);
            remainder = segmentRemainder;
            const auto frontZeroPaddingLength = segmentWidth - segmentQuotient._digits.size();
            quotientString.insert(quotientString.end(), frontZeroPaddingLength, 0);
            quotientString += segmentQuotient._digits;
        }

        return { fromIntString(std::move(quotientString)), remainder };
    }

    // Constructor for internal building of new objects
    UnsignedBigInt() {}
public:
    // Create a new BigInt from a string representation
    explicit UnsignedBigInt(std::string_view value) {
        _digits.reserve(value.size());
        for (auto v : value) {
            _digits.push_back(v - '0');
        }
    }

    // Utility function to build new objects more efficiently
    static UnsignedBigInt fromIntString(DigitString value) {
        UnsignedBigInt result;
        result._digits = std::move(value);

        // Remove leading zeros
        auto it = std::find_if_not(result._digits.begin(), result._digits.end(), [](auto c) { return c == 0; });
        if (it != result._digits.begin()) {
            result._digits.erase(result._digits.begin(), it);
        }
        return result;
    }
    
    // Arithmetic Operators
    UnsignedBigInt operator+(const UnsignedBigInt& other) const {
        DigitString result;
        result.reserve(std::max(_digits.size(),  other._digits.size()));
        DigitType carry = 0;
        DigitStringView one(_digits), two(other._digits);
        while (!one.empty() || !two.empty()) {
            const auto nextDigit = consumeNextDigit(one) + consumeNextDigit(two) + carry;
            result.push_back(nextDigit % BASE);
            carry = nextDigit / BASE;
        }
        if (carry) {
            result.push_back(1);
        }
        std::reverse(result.begin(), result.end());
        return fromIntString(std::move(result));
    }
    UnsignedBigInt operator-(const UnsignedBigInt& other) const {
        DigitString result;
        DigitType borrow = 0;
        DigitStringView one(_digits), two(other._digits);
        while (!one.empty() || !two.empty()) {
            auto nextDigit = consumeNextDigit(one) - consumeNextDigit(two) - borrow;
            if (nextDigit < 0) {
                nextDigit += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.push_back(nextDigit);
        }
        // Remove leading zeros
        while (!result.empty() && result.back() == 0) {
            result.pop_back();
        }
        if (result.empty()) {
            return UnsignedBigInt("0");
        };
        std::reverse(result.begin(), result.end());
        return fromIntString(std::move(result));
    }
    UnsignedBigInt operator*(const UnsignedBigInt& other) const {
        const int64_t oneSize = _digits.size();
        const int64_t twoSize = other._digits.size();
        DigitString result(oneSize + twoSize, 0);

        for (auto i = oneSize-1; i>=0; --i) {
            for (auto j = twoSize-1; j>=0; --j) {
                const auto current = _digits[i] * other._digits[j];
                const auto sum = current + result[i + j + 1];
                result[i + j + 1] = sum % BASE;
                result[i + j] += sum / BASE;
            }
        }
        return UnsignedBigInt::fromIntString(std::move(result));
    }
    UnsignedBigInt operator/(const UnsignedBigInt& other) const {
        return divModLong(*this, other).first;
        }
    UnsignedBigInt operator%(const UnsignedBigInt& other) const {
        return divModLong(*this, other).second;
    }
    void multiplyByTen() {
        assert(BASE == 10);
        _digits.push_back(0);
    }
    void divideByTen() {
        assert(BASE == 10);
        _digits.pop_back();
    }
    void timesBaseToThe(int64_t exponent) {
        if (const bool divide = exponent < 0; divide) {
            _digits.erase(_digits.end() + exponent, _digits.end());
        } else {
            _digits.insert(_digits.end(), exponent, 0);
        }
    }
    // number * 10^exponent
    void timesTenToThe(int64_t exponent) {
        assert(BASE == 10);
        return timesBaseToThe(exponent);
    }

    // Comparison operators
    bool operator==(const UnsignedBigInt& other) const { return _digits == other._digits; }
    bool operator!=(const UnsignedBigInt& other) const { return !(*this == other); }
    bool operator<(const UnsignedBigInt& other) const {
        const auto thisSize = _digits.size();
        const auto otherSize = other._digits.size();
        return std::tie(thisSize, _digits) < std::tie(otherSize, other._digits);
    }
    bool operator<=(const UnsignedBigInt& other) const { return (*this == other) || (*this < other); }
    bool operator>(const UnsignedBigInt& other) const { return (*this != other) && !(*this < other); }
    bool operator>=(const UnsignedBigInt& other) const { return (*this == other) || (*this > other); }

    // Stream out operator
    friend std::ostream& operator<<(std::ostream& os, const UnsignedBigInt& bigInt) {
        for (auto c : bigInt._digits) {
            os << uint64_t(c);
        }
        return os;
    }
};