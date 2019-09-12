//
// Created by Michael on 13.09.2019.
//

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
