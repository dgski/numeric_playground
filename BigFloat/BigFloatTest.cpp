#include "BigFloat.hpp"

auto squareRootBinarySearch(BigFloat x) {
    if (x < BigFloat("0.0")) {
        return BigFloat("-1.0");
    }

    BigFloat::setGlobalPrecision(200);
    
    BigFloat left("0.0");
    BigFloat right = x;
    BigFloat mid("0.0");
    const BigFloat two("2.0");

    while ((right - left) > BigFloat::epsilon()) {
        mid = std::move(left);
        mid += right;
        mid /= two;
        if ((mid * mid) > x) {
            right = std::move(mid);
        } else {
            left = std::move(mid);
        }
    }

    return left;
}

int main() {
    // printing
    BigFloat printTestValue("104242342123123234.2320");
    std::ostringstream ss;
    ss << printTestValue;
    if (ss.str() != "104242342123123234.2320") {
        std::cout << "printTestValue=" << printTestValue << std::endl;
        assert(false);
    }

    // printing 2
    BigFloat printTest2Value("0.0");
    std::ostringstream ss2;
    ss2 << printTest2Value;
    if (ss2.str() != "0.0") {
        std::cout << "printTest2Value=" << printTest2Value << std::endl;
        assert(false);
    }

    // addition
    auto additionTestResult = BigFloat("100.2") + BigFloat("200.3");
    if (additionTestResult != BigFloat("300.5")) {
        std::cout << "additionTestResult=" << additionTestResult << std::endl;
        assert(false);
    }

    // addition 2
    auto addition2TestResult = BigFloat("1.2234234") + BigFloat("2.3");
    if (addition2TestResult != BigFloat("3.5234234")) {
        std::cout << "addition2TestResult=" << addition2TestResult << std::endl;
        assert(false);
    }

    // subtraction
    auto subtractionTestResult = BigFloat("8888.2") - BigFloat("100.3");
    if (subtractionTestResult != BigFloat("8787.9")) {
        std::cout << "subtractionTestResult=" << subtractionTestResult << std::endl;
        assert(false);
    }

    // subtraction 2
    auto subtractionTest2Result = BigFloat("100.0") - BigFloat("200.0");
    if (subtractionTest2Result != BigFloat("-100.0")) {
        std::cout << "subtractionTest2Result" << subtractionTest2Result << std::endl;
        assert(false);
    }

    // multiplication
    auto multiplicationResult = BigFloat("123123431234124.343434234234") * BigFloat("0.5");
    if (multiplicationResult != BigFloat("61561715617062.1717171171170")) {
        std::cout << "multiplicationResult=" << multiplicationResult << std::endl;
        assert(false);
    }

    // division
    //auto divisionResult = BigFloat("10000000.0") / BigFloat("3.0");
    //if (divisionResult != BigFloat("3333333.33333333333333333333333333333333333333330000000000")) {
    //    std::cout << "divisionResult=" << divisionResult << std::endl;
    //    assert(false);
    //}

    auto result = squareRootBinarySearch(BigFloat("42.0"));
    std::cout << "sqrt(42)=" << result << std::endl;

    return 0;
}