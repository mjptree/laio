#ifndef SOCKETADDR_H
#define SOCKETADDR_H

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class SocketAddr {

        };

    } // namespace net

} // namespace laio

#endif // SOCKETADDR_H
