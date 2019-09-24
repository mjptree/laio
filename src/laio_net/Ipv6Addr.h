#ifndef IPV4ADDR_H
#define IPV4ADDR_H

#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class Ipv4Addr {};

    } // namespace net

} // namespace laio

#endif // IPV4ADDR_H
