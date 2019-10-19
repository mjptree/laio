#pragma once

#include <WinSock2.h>
#include <Windows.h>

#include <tuple>
#include <variant>

#include "AcceptAddrBuf.h"
#include "TcpStream.h"

// Symbol defined as __STRUCT__ in <combaseapi.h>
#ifdef interface
    #undef interface
#endif

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net::interface {

        struct TcpListenerExt {

            virtual Result<bool> accept_overlapped(const TcpStream& socket, AcceptAddrBuf& address, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::monostate> accept_complete(const TcpStream& socket) noexcept = 0;
            virtual Result<std::tuple<std::size_t, unsigned long>> result(OVERLAPPED* overlapped) noexcept = 0;
            virtual ~TcpListenerExt() = default;

        };

    } // namespace net::interface

} // namespace laio
