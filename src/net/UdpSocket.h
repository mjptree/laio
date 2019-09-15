#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <WinSock2.h>

#include <variant>
#include "UdpSocketExt.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class UdpSocket : public interface::UdpSocketExt {

        };

    } // namespace net

} // namespace laio

#endif // UDPSOCKET_H
