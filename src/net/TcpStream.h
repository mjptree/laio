#ifndef TCPSTREAM_H
#define TCPSTREAM_H

#include <variant>
#include "TcpStreamExt.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class TcpStream : public interface::TcpStreamExt {

        };

    } // namespace net

} // namespace laio

#endif // TCPSTREAM_H
