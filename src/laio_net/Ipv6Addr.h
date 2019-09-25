#ifndef IPV6ADDR_H
#define IPV6ADDR_H

#include <WS2tcpip.h>

#include <variant>
#include "IpAddr.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class Ipv6Addr : public interface::IpAddr {

            IN6_ADDR _raw_ipv6_address;

        public:

            explicit constexpr Ipv6Addr() noexcept
                : _raw_ipv6_address{} {}

            /// TODO: To be implemented
            [[nodiscard]] bool is_unspecified() const noexcept override {
                    return true;
            }

            /// TODO: To be implemented
            [[nodiscard]] bool is_loopback() const noexcept override {
                    return true;
            }

            /// TODO: To be implemented
            [[nodiscard]] bool is_global() const noexcept override {
                    return true;
            }

            /// TODO: To be implemented
            [[nodiscard]] bool is_multicast() const noexcept override {
                    return true;
            }

            /// TODO: To be implemented
            [[nodiscard]] bool is_documentation() const noexcept override {
                    return true;
            }

            /// Return `false`, as this IP address is implemented as IPv6
            [[nodiscard]] bool is_ipv4() const noexcept override {
                    return false;
            }

            /// Return `true`, as this IP address is implemented as IPv6
            [[nodiscard]] bool is_ipv6() const noexcept override {
                    return true;
            }

        };

    } // namespace net

} // namespace laio

#endif // IPV6ADDR_H
