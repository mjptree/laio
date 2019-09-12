#ifndef SOCKETADDRBUF_H
#define SOCKETADDRBUF_H

#include <WinSock2.h>

namespace laio::net {

    class SocketAddrBuf {

        SOCKADDR_STORAGE _socket_address_buffer;
        int _length;

        // TODO: - Implement copy assignment operator

    };

} // namespace laio::net

#endif // SOCKETADDRBUF_H
