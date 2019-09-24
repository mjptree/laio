#ifndef IPADDR_H
#define IPADDR_H

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class IpAddr {};

    } // namespace net

} // namespace laio

#endif // IPADDR_H
