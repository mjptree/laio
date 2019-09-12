#ifndef TCPSTREAM_H
#define TCPSTREAM_H

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class TcpStream {

        };

    } // namespace net

} // namespace laio

#endif // TCPSTREAM_H
