#pragma once

#include <ws2tcpip.h>

#include <cstdint>
#include <variant>

#include "fmt/format.h"

#include "Ipv6Addr.h"

namespace laio {

    using std::uint16_t;

    namespace net {

        /// IPv6 socket address implementation
        ///
        /// \details IPv6 socket address consisting of an IPv6 address, a 16-bit port number, the traffic class, flow
        /// label, and a scope identifier. This implementation assumes a host system with little-endian byte order.
        class SocketAddrV6 {

            SOCKADDR_IN6 inner_{};    ///< Windows IPv6 socket address structure

        public:
            // # Constructors
            constexpr SocketAddrV6() noexcept = default;

            constexpr SocketAddrV6(SOCKADDR_IN6 socketAddrV6) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                : inner_{std::move(socketAddrV6)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

            SocketAddrV6(const SocketAddrV6& other) noexcept = default;

            SocketAddrV6(SocketAddrV6&& other) noexcept
                : inner_{std::move(other.inner_)} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            {
                other.inner_.sin6_family = 0;
                other.inner_.sin6_port = 0;
                other.inner_.sin6_flowinfo = 0;
                for (uint16_t& elem: other.inner_.sin6_addr.u.Word) {
                    elem = 0;
                }
                other.inner_.sin6_scope_id = 0;

            }

            explicit SocketAddrV6(const Ipv6Addr& ip, uint16_t port, uint32_t flowInfo, uint32_t scopeId) noexcept
                : inner_{AF_INET6, static_cast<uint16_t>((port << 8u) | (port >> 8u)), flowInfo, ip, {scopeId}} {} // NOLINT(hicpp-signed-bitwise)

            // # Operator Overloads
            constexpr operator SOCKADDR_IN6() const noexcept { // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                return inner_;
            }

            explicit operator std::string() const noexcept {
                return fmt::format("[{}]:{}", ip(), port());
            }

            bool operator==(const SocketAddrV6& rhs) const noexcept {
                return inner_.sin6_port == rhs.inner_.sin6_port
                    && inner_.sin6_addr.u.Word == rhs.inner_.sin6_addr.u.Word;
            }

            // # Public member functions

            [[nodiscard]] Ipv6Addr ip() const noexcept {
                return inner_.sin6_addr;
            }

            void set_ip(const Ipv6Addr& ip) noexcept {
                inner_.sin6_addr = std::move(ip); // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            }

            [[nodiscard]] uint16_t port() const noexcept {
                uint16_t port = inner_.sin6_port;
                return (port << 8u | port >> 8u); // NOLINT(hicpp-signed-bitwise)
            }

            void set_port(uint16_t port) noexcept {
                inner_.sin6_port = (port << 8u | port >> 8u); // NOLINT(hicpp-signed-bitwise)
            }

            [[nodiscard]] uint32_t flow_info() const noexcept {
                return inner_.sin6_flowinfo;
            }

            void flow_info(uint32_t flowInfo) noexcept {
                inner_.sin6_port = flowInfo;
            }

            [[nodiscard]] uint32_t scope_id() const noexcept {
                return inner_.sin6_scope_id;
            }

            void set_scope_id(uint32_t scopeId) noexcept {
                inner_.sin6_scope_id = scopeId;
            }

        }; // class SocketAddrV6

    }

} // namespace laio