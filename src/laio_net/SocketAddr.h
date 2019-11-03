#pragma once

#include "win_error.h"

#include "SocketAddrV4.h"
#include "SocketAddrV6.h"

// Symbol defined as __STRUCT__ in <combaseapi.h>
#ifdef interface
    #undef interface
#endif

namespace laio {

    namespace net::interface {

        /// Socket address interface, implemented either for IpV4 or IpV6 address types
        ///
        ///
        template<typename derived>
        struct SocketAddr {

        };

    }

} // namespace laio::net