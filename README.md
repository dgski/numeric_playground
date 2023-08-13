# Numeric Playground

Working on various big number approaches in C++.

- [BigInt](./BigInt/BigInt.hpp): A Simple BigInt implementation. Potential improvements:
    - Provide direct mutation operations (rather than always creating an allocated a new object)
    - Include optional statically allocated buffer.
    - Small int optimization (If fits inside int64_t, just use that)
    - Do not use base10; could be more efficient to use the full char or potentially even long long to store the digits. 