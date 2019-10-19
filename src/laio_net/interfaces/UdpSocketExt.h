#pragma once

#include <Windows.h>

#include <optional>
#include <tuple>
#include <variant>

#include "gsl/span"

#include "SocketAddr.h"
#include "SocketAddrBuf.h"

// Symbol defined as __STRUCT__ in <combaseapi.h>
#ifdef interface
    #undef interface
#endif

namespace laio {

    using std::uint8_t;

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net::interface {

        struct UdpSocketExt {

            virtual Result<std::optional<std::size_t>> recv_from_overlapped(gsl::span<uint8_t> buf, SocketAddrBuf* address, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> recv_overlapped(gsl::span<uint8_t> buf, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> send_to_overlapped(gsl::span<const uint8_t> buf, SocketAddr& address, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> send_overlapped(gsl::span<const uint8_t> buf, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::tuple<std::size_t, unsigned long>> result(OVERLAPPED* overlapped) noexcept = 0;
            virtual ~UdpSocketExt() = default;

        };

    } // namespace net::interface

} // namespace laio
