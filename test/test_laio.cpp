#include "catch2/catch.hpp"

#include <iostream>
#include "../src/CompletionPort.h"

TEST_CASE() {
    std::cout << "Size of std::size_t: " << sizeof(std::size_t) << '\n'
        << "Size of ULONG_PTR: " << sizeof(ULONG_PTR) << '\n'
        << "Size of DWORD: " << sizeof(DWORD) << '\n'
        << "Size of short: " << sizeof(unsigned short) << '\n'
        << "Size of short int: " << sizeof(unsigned short int) << '\n'
        << "Size of int: " << sizeof(unsigned int) << '\n'
        << "Size of long int: " << sizeof(unsigned long int) << '\n'
        << "Size of long: " << sizeof(unsigned long) << '\n'
        << "Size of long long: " << sizeof(unsigned long long) << std::endl;
    CHECK(sizeof(std::size_t) == sizeof(ULONG_PTR));
    CHECK(sizeof(unsigned int) == sizeof(unsigned long));
}

#include "traits.h"

struct test {};

template<>
constexpr bool laio::trait::send<test> = true;

TEST_CASE() {
    using namespace laio;
    CHECK(trait::send<test>);
}