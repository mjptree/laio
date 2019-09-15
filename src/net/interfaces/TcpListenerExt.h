#ifndef  TCPLISTENEREXT_H
#define  TCPLISTENEREXT_H

#include <WinSock2.h>

#include <variant>
#include <tuple>
#include <Windows.h>
#include "TcpStream.h"
#include "AcceptAddrBuf.h"

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
            // TODO: Set destructor to default (mark as virtual?)

        };

    } // namespace net::interface

} // namespace laio

#endif // TCPLISTENEREXT_H
