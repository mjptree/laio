#ifndef IPV4ADDR_H
#define IPV4ADDR_H

#include <variant>
#include "IpAddr.h"

namespace laio {

    using std::uint8_t;

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class Ipv4Addr : public interface::IpAddr {

            IN_ADDR _raw_ipv4_address;

        public:

            explicit constexpr Ipv4Addr(const uint8_t a, const uint8_t b, const uint8_t c, const uint8_t d) noexcept
                : _raw_ipv4_address{ /* s_addr */ } {}

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

            /// Return `true`, as this IP address is implement as IPv4
            [[nodiscard]] bool is_ipv4() const noexcept override {
                return true;
            }

            /// Return `false`, as this IP address is implement as IPv4
            [[nodiscard]] bool is_ipv6() const noexcept override {
                return false;
            }

        };

    } // namespace net

} // namespace laio

#endif // IPV4ADDR_H
