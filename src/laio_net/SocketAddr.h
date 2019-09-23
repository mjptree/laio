#ifndef SOCKETADDR_H
#define SOCKETADDR_H

#include "SocketAddrV4.h"
#include "SocketAddrV6.h"

namespace laio::net {

        class SocketAddr {
            std::variant<SocketAddrV4, SocketAddrV6> _;

        };

} // namespace laio::net

#endif // SOCKETADDR_H
