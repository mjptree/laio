#ifndef  UDPSOCKETEXT_H
#define  UDPSOCKETEXT_H

#include <variant>
#include <optional>
#include <tuple>
#include <Windows.h>
#include "SocketAddrBuf.h"
#include "SocketAddr.h"

// Symbol defined as __STRUCT__ in <combaseapi.h>
#ifdef interface
    #undef interface
#endif

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net::interface {

        struct UdpSocketExt {

            virtual Result<std::optional<std::size_t>> recv_from_overlapped(unsigned char buf[], SocketAddrBuf* address, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> recv_overlapped(unsigned char buf[], OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> send_to_overlapped(const unsigned char buf[], SocketAddr& address, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> send_overlapped(const unsigned char buf[], OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::tuple<std::size_t, unsigned long>> result(OVERLAPPED* overlapped) noexcept = 0;
            virtual ~UdpSocketExt() = default;

        };

    } // namespace net::interface

} // namespace laio

#endif // UDPSOCKETEXT_H
