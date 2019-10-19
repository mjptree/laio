#pragma once

#include <WS2tcpip.h>

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class SocketAddrV6 {

            SOCKADDR_IN6 inner_;

        public:
            explicit constexpr SocketAddrV6(SOCKADDR_IN6 socketAddrV6) noexcept
                : inner_{std::move(socketAddrV6)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

        };

    }

} // namespace laio