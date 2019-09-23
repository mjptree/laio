#ifndef ACCEPTADDR_H
#define ACCEPTADDR_H

#include "AcceptAddrBuf.h"

namespace laio::net {

    // Requires declaration due to cyclic inclusion of header files, definition can be found in <AcceptAddrBuf.h>
    class AcceptAddrBuf;

    class AcceptAddr {

        SOCKADDR* _local_socket_address;
        int _local_length;
        SOCKADDR* _remote_socket_address;
        int _remote_length;
        AcceptAddrBuf* _data;

    };

} // namespace laio::net

#endif // ACCEPTADDR_H
