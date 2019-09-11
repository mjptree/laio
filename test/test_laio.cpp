#include "catch2/catch.hpp"

#include <iostream>
#include "../src/CompletionPort.h"
#include "../src/Handle.h"
#include "win_error.h"

TEST_CASE() {
    HANDLE _in_handle = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE _out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    laio::Handle handleIn{_in_handle};
    laio::Handle handleOut{_out_handle};
    std::cout << _in_handle << '\n'
        << handleIn.raw() << '\n';
    std::cout << _out_handle << '\n'
              << handleOut.raw() << '\n';

    HANDLE _move_out{std::move(handleIn).into_raw()};
    std::cout << handleIn.raw() << std::endl;
    std::cout << _move_out << std::endl;
    if (wse::win_error err{}; !(err == wse::win_errc::success_exception)) {
        std::cout << err.what() << std::endl;
    }
}

#include "traits.h"

struct test {};

template<>
constexpr bool laio::trait::send<test> = true;

TEST_CASE() {
    using namespace laio;
    CHECK(trait::send<test>);
}