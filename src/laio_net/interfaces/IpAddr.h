#ifndef IPADDR_H
#define IPADDR_H

#include <WinSock2.h>

#include <variant>

// Symbol defined as __STRUCT__ in <combaseapi.h>
#ifdef interface
    #undef interface
#endif

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net::interface {

        /// An IP address interface, implemented either as IPv4 or IPv6
        ///
        /// In the Rust Standard Library this has been implemented as an enum with fields, that can hold
        /// either versions of IP addressing. As the relevant method signatures for both implementations
        /// are equal, we can circumvent all the pattern matching by inheriting from a common interface.
        struct IpAddr {

            /// Return `true` if IpAddr is set to `unspecified` address
            [[nodiscard]] virtual bool is_unspecified() const noexcept = 0;

            /// Return `true` if IpAddr is set to a loopback address
            [[nodiscard]] virtual bool is_loopback() const noexcept = 0;

            /// Return `true` if IpAddr is within a globally routable range
            [[nodiscard]] virtual bool is_global() const noexcept = 0;

            /// Return `true` if IpAddr is set to a multicast address
            [[nodiscard]] virtual bool is_multicast() const noexcept = 0;

            /// Return `true` if IpAddr is within a range designated for documentation
            [[nodiscard]] virtual bool is_documentation() const noexcept = 0;

            /// Returns `true` if IpAddr is implemented as an Ipv4Addr
            [[nodiscard]] virtual bool is_ipv4() const noexcept = 0;

            /// Returns `true` if IpAddr is implemented as an Ipv6Addr
            [[nodiscard]] virtual bool is_ipv6() const noexcept = 0;

            virtual ~IpAddr() = default;

        };

    } // namespace net::interface

} // namespace laio

#endif // IPADDR_H
