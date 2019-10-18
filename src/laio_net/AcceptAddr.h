#ifndef ACCEPTADDR_H
#define ACCEPTADDR_H

#include "AcceptAddrBuf.h"

namespace laio::net {

    // Requires declaration due to cyclic inclusion of header files, definition can be found in <AcceptAddrBuf.h>
    class AcceptAddrBuf;

    class AcceptAddr {

        SOCKADDR* local_socket_address_{};
        int local_length_{};
        SOCKADDR* remote_socket_address_{};
        int remote_length_{};
        AcceptAddrBuf* data_{};

    public:
        AcceptAddr(SOCKADDR* localAddress,
                int localLength,
                SOCKADDR* remoteAddress,
                int remoteLength,
                AcceptAddrBuf* addressBuffer) noexcept
            : local_socket_address_{std::move(localAddress)},   // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            local_length_{std::move(localLength)},              // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            remote_socket_address_{std::move(remoteAddress)},   // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            remote_length_{std::move(remoteLength)},            // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            data_{std::move(addressBuffer)}                     // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
        {}

    };

} // namespace laio::net

#endif // ACCEPTADDR_H
