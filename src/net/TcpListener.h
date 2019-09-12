#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class TcpListener {

        };

    } // namespace net

} // namespace laio

#endif // TCPLISTENER_H
