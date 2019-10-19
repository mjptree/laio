#pragma once

#include <Windows.h>

#include <optional>
#include <tuple>
#include <variant>

#include "gsl/span"

#include "SocketAddr.h"

// Symbol defined as __STRUCT__ in <combaseapi.h>
#ifdef interface
    #undef interface
#endif

namespace laio {

    using std::uint8_t;

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net::interface {

        struct TcpStreamExt {

            virtual Result<std::optional<std::size_t>> read_overlapped(gsl::span<uint8_t> buf, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> write_overlapped(gsl::span<const uint8_t> buf, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> connect_overlapped(const SocketAddr& address, gsl::span<const uint8_t> buf, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::monostate> connect_complete() noexcept = 0;
            virtual Result<std::tuple<std::size_t, unsigned long>> recv_overlapped(gsl::span<uint8_t> buf, OVERLAPPED* overlapped) noexcept = 0;
            virtual ~TcpStreamExt() = default;

        };

    } // namespace net::interface

} // namespace laio