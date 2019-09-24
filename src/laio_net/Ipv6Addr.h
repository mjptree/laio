#ifndef IPV6ADDR_H
#define IPV6ADDR_H

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class Ipv6Addr {};

    } // namespace net

} // namespace laio

#endif // IPV6ADDR_H
