#include "BigFloat.hpp"

int main() {
    assert(UnsignedBigFloat("1.5") + UnsignedBigFloat("1.25") == UnsignedBigFloat("2.75"));
    assert(UnsignedBigFloat("1.6") - UnsignedBigFloat("1.2") == UnsignedBigFloat("0.4"));
    assert(UnsignedBigFloat("2.0") * UnsignedBigFloat("4.5") == UnsignedBigFloat("9.0"));
    assert(UnsignedBigFloat("9.0") / UnsignedBigFloat("2.0") == UnsignedBigFloat("4.5"));
    assert((UnsignedBigFloat("6.5625") - UnsignedBigFloat("6.48046875")) == UnsignedBigFloat("0.08203125"));
    assert(UnsignedBigFloat("112391293712987391287398127.0") > UnsignedBigFloat("8888888.888"));
    assert(UnsignedBigFloat("1.0") != UnsignedBigFloat("3.888"));
    assert(UnsignedBigFloat("999.999") + UnsignedBigFloat("2.0") == UnsignedBigFloat("1001.999"));
    assert(sqrt(UnsignedBigFloat("10.0")) == UnsignedBigFloat("3.16227766"));
    return 0;
}