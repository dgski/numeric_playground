#include "BigInt.hpp"

int main() {
    assert(UnsignedBigInt("1") + UnsignedBigInt("100") == UnsignedBigInt("101"));
    assert(UnsignedBigInt("100000") - UnsignedBigInt("9999") == UnsignedBigInt("90001"));
    assert(UnsignedBigInt("20") * UnsignedBigInt("5") == UnsignedBigInt("100"));
    assert(UnsignedBigInt("200") / UnsignedBigInt("30") == UnsignedBigInt("6"));
    assert(UnsignedBigInt("909") / UnsignedBigInt("9") == UnsignedBigInt("101"));
    assert(UnsignedBigInt("21000000") / UnsignedBigInt("20") == UnsignedBigInt("1050000"));
    assert(UnsignedBigInt("1") != UnsignedBigInt("11"));
    assert(UnsignedBigInt("200") > UnsignedBigInt("30"));
    assert(UnsignedBigInt("100") < UnsignedBigInt("130"));
    assert(UnsignedBigInt("201") % UnsignedBigInt("200") == UnsignedBigInt("1"));
    return 0;
}