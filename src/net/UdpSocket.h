#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class UdpSocket {

        };

    } // namespace net

} // namespace laio

#endif // UDPSOCKET_H
