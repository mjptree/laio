#ifndef SOCKETADDRV4_H
#define SOCKETADDRV4_H

#include <WinSock2.h>

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class SocketAddrV4 {

            SOCKADDR_IN _inner;

        public:
            explicit constexpr SocketAddrV4(const SOCKADDR_IN& socketAddrV4) noexcept
                : _inner{socketAddrV4} {}

            explicit constexpr SocketAddrV4(SOCKADDR_IN&& socketAddrV4) noexcept
                : _inner{std::move(socketAddrV4)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

        };

    }

} // namespace laio

#endif // SOCKETADDRV4_H
