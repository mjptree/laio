#pragma once

#include <WinSock2.h>

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class SocketAddrV4 {

            SOCKADDR_IN inner_;

        public:
            explicit constexpr SocketAddrV4(SOCKADDR_IN socketAddrV4) noexcept
                : inner_{std::move(socketAddrV4)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

        };

    }

} // namespace laio