#ifndef ACCEPTADDRBUF_H
#define ACCEPTADDRBUF_H

#include <WinSock2.h>

#include <variant>

#include "AcceptAddr.h"
#include "TcpListener.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class AcceptAddrBuf {

            SOCKADDR_STORAGE local_socket_address_buffer_{};
            SOCKADDR_STORAGE remote_socket_address_buffer_{};

        public:

            constexpr AcceptAddrBuf() noexcept = default;

            static AcceptAddrBuf create() noexcept {
                return AcceptAddrBuf{};
            }

            Result<AcceptAddr> parse(TcpListener& socket) noexcept {
                AcceptAddr ret{
                    nullptr,
                    0,
                    nullptr,
                    0,
                    this,
                };
                return ret;
            }
        };

    } // namespace net

} // namespace laio

#endif // ACCEPTADDRBUF_H
