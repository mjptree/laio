#ifndef SOCKETADDRV6_H
#define SOCKETADDRV6_H

#include <WS2tcpip.h>

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class SocketAddrV6 {

            SOCKADDR_IN6 _inner;

        public:
            explicit constexpr SocketAddrV6(const SOCKADDR_IN6& socketAddrV6) noexcept
                    : _inner{socketAddrV6} {}

            explicit constexpr SocketAddrV6(SOCKADDR_IN6&& socketAddrV6) noexcept
            : _inner{std::move(socketAddrV6)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

        };

    }

} // namespace laio

#endif // SOCKETADDRV6_H
