#pragma once

#include <WS2tcpip.h>

#include <variant>

#include "IpAddr.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        enum class Ipv6MulticastScope {
            InterfaceLocal,
            LinkLocal,
            RealmLocal,
            AdminLocal,
            SiteLocal,
            OrganizationLocal,
            Global
        };

        class Ipv6Addr final : public interface::IpAddr {

            IN6_ADDR raw_ipv6_address_;

        public:

            explicit constexpr Ipv6Addr() noexcept
                : raw_ipv6_address_{} {}

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
