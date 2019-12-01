#pragma once

#include <winsock2.h>

#include <cstdint>
#include <utility>
#include <variant>

#include "fmt/format.h"
#include "win_error.h"

#include "Ipv4Addr.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, wse::win_error>;

    namespace net {

        /// IPv4 socket address implementation
        ///
        /// \details IPv4 socket address consisting of an IPv4 address, and a 16-bit port number. This implementation
        /// assumes a host system with little-endian byte order.
        class SocketAddrV4 {

            SOCKADDR_IN inner_{};    ///< Windows IPv4 socket address structure

        public:
            // # Constructors
            constexpr SocketAddrV4() noexcept = default;

            constexpr SocketAddrV4(SOCKADDR_IN socketAddrV4) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                : inner_{std::move(socketAddrV4)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

            SocketAddrV4(const SocketAddrV4& other) noexcept = default;

            SocketAddrV4(SocketAddrV4&& other) noexcept
                : inner_{std::move(other.inner_)} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            {
                other.inner_.sin_family = 0;
                other.inner_.sin_port = 0;
                other.inner_.sin_addr.S_un.S_addr = 0;
                for (char& elem: other.inner_.sin_zero) {
                    elem = 0;
                }
            }

            explicit SocketAddrV4(const Ipv4Addr& ip, uint16_t port) noexcept
                : inner_{AF_INET, static_cast<u_short>((port >> 8u) | (port << 8u)), ip, {0}} {} // NOLINT(hicpp-signed-bitwise)

            // # Operator overloads
            constexpr operator SOCKADDR_IN() const noexcept { // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                return inner_;
            }

            explicit operator std::string() const noexcept {
                return fmt::format("{}:{}", ip(), port());
            }

            bool operator==(const SocketAddrV4& rhs) const noexcept {
                return inner_.sin_port == rhs.inner_.sin_port
                    && inner_.sin_addr.S_un.S_addr == rhs.inner_.sin_addr.S_un.S_addr;
            }

            // # Public member functions

            /// Return IP address of this socket address
            [[nodiscard]] Ipv4Addr ip() const noexcept {
                return inner_.sin_addr;
            }

            /// Change IP address of this socket address
            ///
            /// \param ip IP address associated with this socket address
            void set_ip(const Ipv4Addr& ip) noexcept {
                inner_.sin_addr = std::move(ip); // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            }

            /// Return port number of this socket address
            [[nodiscard]] uint16_t port() const noexcept {
                uint16_t port = inner_.sin_port;
                return (port << 8u | port >> 8u); // NOLINT(hicpp-signed-bitwise)
            }

            /// Change port number of this socket address
            ///
            /// \param port Port number associated with this socket address
            void set_port(uint16_t port) noexcept {
                inner_.sin_port = (port << 8u | port >> 8u); // NOLINT(hicpp-signed-bitwise)
            }

        }; // class SocketAddrV4

    } // namespace net

} // namespace laio