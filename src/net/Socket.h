#ifndef SOCKET_H
#define SOCKET_H

#include <WinSock2.h>

#include <variant>
#include <chrono>
#include "SocketAddr.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class Socket {
            SOCKET _raw_socket;
        public:
            explicit constexpr Socket(const SOCKET& socket) noexcept
                : _raw_socket{socket} {}

            explicit constexpr Socket(SOCKET&& socket) noexcept
                : _raw_socket{std::move(socket)} {}

            static void init() noexcept;

            static Result<Socket> create(const SocketAddr& address, const int type) noexcept;

            Result<std::monostate> connect_timeout(const SocketAddr& addr, const std::chrono::nanoseconds timeout) noexcept;

            Result<Socket> accept(SOCKADDR* storage, int len) noexcept;

            Result<Socket> duplicate() noexcept;

            Result<std::size_t> recv_with_flags(unsigned char buf[], int flags) noexcept;

            Result<std::size_t> read(unsigned char buf[]) noexcept;

            Result<std::size_t> read_vectored(IoSliceMut bufs[]) noexcept;
        };

    } // namespace net

} // namespace laio

#endif // SOCKET_H
