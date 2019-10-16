#ifndef ACCEPTADDRBUF_H
#define ACCEPTADDRBUF_H

#include <WinSock2.h>


#include <variant>
#include "AcceptAddr.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class AcceptAddrBuf {

            struct {
                SOCKADDR_STORAGE _local_socket_address_buffer;
                unsigned char _padding_1[16];
                SOCKADDR_STORAGE _remote_socket_address_buffer;
                unsigned char _padding_2[16];
            } _inner{};

        public:

            AcceptAddrBuf() : _inner{} {}

            //static AcceptAddrBuf create() noexcept;

            //Result<AcceptAddr> parse() noexcept;
        };

    } // namespace net

} // namespace laio

#endif // ACCEPTADDRBUF_H
