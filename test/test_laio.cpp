#include "catch2/catch.hpp"

#include "../net/AcceptAddrBuf.h"
#include <iostream>
#include "../src/CompletionPort.h"
#include "../src/Handle.h"
#include "win_error.h"

TEST_CASE() {

}

TEST_CASE() {
    laio::net::AcceptAddrBuf buf{};
    CHECK(sizeof buf == sizeof(laio::net::AcceptAddrBuf));
    std::cout << sizeof(SOCKADDR_STORAGE) << std::endl;
    std::cout << alignof(laio::net::AcceptAddrBuf) << std::endl;
    unsigned char bytes [sizeof buf];
    std::memcpy(bytes, &buf, sizeof buf);
    for (int i = 0; i < sizeof buf; ++i)
        std::printf("%x", bytes[i]);
    std::cout << std::endl;
    LPSOCKADDR lpSockAddr{};
    SOCKADDR* sockAddr = lpSockAddr;
    std::cout << sockAddr << '\n' << lpSockAddr << std::endl;
}