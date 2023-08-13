#include "BigFloat.hpp"

int main() {
    // printing
    BigFloat printTestValue("104242342123123234.2320");
    std::ostringstream ss;
    ss << printTestValue;
    if (ss.str() != "104242342123123234.2320") {
        std::cout << "printTestValue=" << printTestValue << std::endl;
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

    // multiplication
    auto multiplicationResult = BigFloat("123123431234124.343434234234") * BigFloat("0.5");
    if (multiplicationResult != BigFloat("61561715617062.1717171171170")) {
        std::cout << "multiplicationResult=" << multiplicationResult << std::endl;
        assert(false);
    }

    // division
    auto divisionResult = BigFloat("10000000.0000000000000") / BigFloat("3.0000000000000");
    if (divisionResult != BigFloat("33.3")) {
        std::cout << "divisionResult=" << divisionResult << std::endl;
        assert(false);
    }


    return 0;
}