#include "BigInt.hpp"

int main() {
    // Addition
    const auto additionResult = (BigInt("901") + BigInt("100"));
    if (additionResult != BigInt("1001")) {
        std::cout << "additionResult=" << additionResult << std::endl;
        assert(false);
    }

    // Addition 2
    const auto addition2Result =
        BigInt("100000000000000000000000000000000000000000000000000000000000000000000000000000") +
        BigInt("100000000000000000000000000000000000000000000000000000000000000000000000000000");
    const auto additionExpected2Result =
        BigInt("200000000000000000000000000000000000000000000000000000000000000000000000000000");
    if (addition2Result != additionExpected2Result) {
        std::cout << "addition2Result=" << addition2Result << std::endl;
        assert(false);
    }

    // Subtraction
    const auto subtractionResult = (BigInt("1000") - BigInt("900"));
    if (subtractionResult != BigInt("100")) {
        std::cout << "subtractionResult=" << subtractionResult << std::endl;
        assert(false);
    }

    // Subtraction 2
    const auto subtraction2Result = (BigInt("1000") - BigInt("2001"));
    if (subtraction2Result != BigInt("-1001")) {
        std::cout << "subtraction2Result=" << subtraction2Result << std::endl;
        assert(false);
    }

    // Subtraction 3
    const auto subtraction3Result = (BigInt("-1000") - BigInt("-2001"));
    if (subtraction3Result != BigInt("1001")) {
        std::cout << "subtraction3Result=" << subtraction3Result << std::endl;
        assert(false);
    }

    // Less-than
    const auto lessThanResult = BigInt("1000000000000000000") < BigInt("2000000000000000000");
    if (!lessThanResult) {
        std::cout << "lessThanResult=" << lessThanResult << std::endl;
        assert(false);
    }

    // Less-than 2
    const auto lessThan2Result = BigInt("1000000000000000000") < BigInt("-20000000000000000000000000000000000000");
    if (lessThan2Result) {
        std::cout << "lessTha2nResult=" << lessThan2Result << std::endl;
        assert(false);
    }

    // Negatives
    const auto negativeResult = (BigInt("1000") + BigInt("-2000"));
    if (negativeResult != BigInt("-1000")) {
        std::cout << "negativeResult=" << negativeResult << std::endl;
        assert(false);
    }

    // Negatives 2
    const auto negative2Result = (BigInt("1000") - BigInt("2000"));
    if (negative2Result != BigInt("-1000")) {
        std::cout << "negative2Result=" << negative2Result << std::endl;
        assert(false);
    }

    // Negatives 3
    const auto negative3Result = (BigInt("1000") - BigInt("-2000"));
    if (negative3Result != BigInt("3000")) {
        std::cout << "negative3Result=" << negative3Result << std::endl;
        assert(false);
    }

    // Multiplication
    const auto multiplicationResult = (BigInt("200") * BigInt("6"));
    if (multiplicationResult != BigInt("1200")) {
        std::cout << "multiplicationResult=" << multiplicationResult << std::endl;
        assert(false);
    }

    // Multiplication 2
    const auto multiplication2Result = (BigInt("-20000") * BigInt("6"));
    if (multiplication2Result != BigInt("-120000")) {
        std::cout << "multiplication2Result=" << multiplication2Result << std::endl;
        assert(false);
    }

    // Division
    const auto divisionResult = (BigInt("-1000") / BigInt("5"));
    if (divisionResult != BigInt("-200")) {
        std::cout << "divisionResult=" << divisionResult << std::endl;
        assert(false);
    }

    // Combined
    // (1000 / 5) + 100000 - (5555555 *2)
    const auto combinedResult = (BigInt("1000") / BigInt("5")) + BigInt("100000") - (BigInt("5555555") * BigInt("2"));
    if (combinedResult != BigInt("-11010910")) {
        std::cout << "combinedResult=" << combinedResult << std::endl;
        assert(false);
    }

    // Overflow
    const auto overflowedResult = BigInt("9223372036854775807") + BigInt("1");
    if (overflowedResult != BigInt("9223372036854775808")) {
        std::cout << "overflowedResult=" << overflowedResult << std::endl;
        assert(false);
    }

    // Big Multiplication
    const auto bigMultiplicationResult =
        BigInt(
            "10000000000000000000000000000000000000000000000000000000000000000"
            "00000000000000000000000000000050000000000000000000000000000000000"
            "00000000000000000000000000000000000000000000000000000000000000000") *
        BigInt(
            "30000000000000000000000000000000000000000000000000000000000000000"
            "00000000000000000000000000000000000000000000000000000000000000000"
            "00000000000000000000000000000000000000000000000000000000000000000");
    const auto bigMultiplicationExpectedResult = BigInt(
        "3000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000001500000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000");
    if (bigMultiplicationResult != bigMultiplicationExpectedResult) {
        std::cout << "bigMultiplicationResult=" << bigMultiplicationResult << std::endl;
        assert(false);
    }

    // Big Division
    const auto bigDivisionResult =
        BigInt(
            "10000000000000000000000000000000000000000000000000000000000000000"
            "00000000000000000000000000000000000000000000000000000000000000000"
            "00000000000000000000000000000000000000000000000000000000000000000") /
        BigInt("100000000");
    const auto bigDivisionExpectedResult = BigInt(
            "10000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000"
            "00000000000000000000000000000000000000000000000000000000");
    if (bigDivisionResult != bigDivisionExpectedResult) {
        std::cout << "bigDivisionResult=" << bigDivisionResult << std::endl;
        assert(false);
    }

    return 0;
}