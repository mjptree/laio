#ifndef IPADDR_H
#define IPADDR_H

#include <variant>

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
            virtual bool is_unspecified() = 0;

            /// Return `true` if IpAddr is set to a loopback address
            virtual bool is_loopback() = 0;

            /// Return `true` if IpAddr is within a globally routable range
            virtual bool is_global() = 0;

            /// Return `true` if IpAddr is set to a multicast address
            virtual bool is_multicast() = 0;

            /// Return `true` if IpAddr is within a range designated for documentation
            virtual bool is_documentation() = 0;

            /// Returns `true` if IpAddr is implemented as an Ipv4Addr
            virtual bool is_ipv4() = 0;

            /// Returns `true` if IpAddr is implemented as an Ipv6Addr
            virtual bool is_ipv6() = 0;

            virtual ~IpAddr() = default;

        };

    } // namespace net::interface

} // namespace laio

#endif // IPADDR_H
