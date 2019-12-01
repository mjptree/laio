#pragma once

#include <variant>

#include "win_error.h"

#include "SocketAddrV4.h"
#include "SocketAddrV6.h"

// Symbol defined as __STRUCT__ in <combaseapi.h>
#ifdef interface
    #undef interface
#endif

namespace laio {

    template<typename T>
    using Result = std::variant<T, wse::win_error>;

    namespace net::interface {

        /// Socket address interface, implemented either for IpV4 or IpV6 address types
        ///
        ///
        template<typename Derived>
        struct SocketAddr {

        };

    }

} // namespace laio::net