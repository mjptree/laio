#ifndef SOCKETADDRBUF_H
#define SOCKETADDRBUF_H

#include <WinSock2.h>

namespace laio::net {

    class SocketAddrBuf {

        SOCKADDR_STORAGE socket_address_buffer_;
        int length_;

        // TODO: - Implement copy assignment operator

    };

} // namespace laio::net

#endif // SOCKETADDRBUF_H
