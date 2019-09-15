#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <variant>
#include "TcpListenerExt.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

    class TcpListener : public interface::TcpListenerExt {

        };

    } // namespace net

} // namespace laio

#endif // TCPLISTENER_H
