#ifndef  TCPSTREAMEXT_H
#define  TCPSTREAMEXT_H

#include <variant>
#include <optional>
#include <tuple>
#include <Windows.h>
#include "SocketAddr.h"

// Symbol defined as __STRUCT__ in <combaseapi.h>
#ifdef interface
    #undef interface
#endif

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net::interface {

        struct TcpStreamExt {

            virtual Result<std::optional<std::size_t>> read_overlapped(unsigned char buf[], OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> write_overlapped(const unsigned char buf[], OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> connect_overlapped(const SocketAddr& address, const unsigned char buf[], OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::monostate> connect_complete() noexcept = 0;
            virtual Result<std::tuple<std::size_t, unsigned long>> recv_overlapped(unsigned char buf[], OVERLAPPED* overlapped) noexcept = 0;
            // TODO: Set destructor to default (mark as virtual?)

        };

    } // namespace net::interface

} // namespace laio

#endif // TCPSTREAMEXT_H
