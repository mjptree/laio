#pragma once

#include <WinSock2.h>

#include <variant>

#include "win_error.h"

// Symbol defined as __STRUCT__ in <combaseapi.h>
#ifdef interface
    #undef interface
#endif

namespace laio {

    template<typename T>
    using Result = std::variant<T, wse::win_error>;

    namespace net::interface {

        /// IP address interface, implemented either as IPv4 or IPv6
        ///
        /// \details In the Rust Standard Library this has been implemented as an enum with fields, that can hold either
        /// versions of IP addressing. As the relevant method signatures for both implementations are equal, we can
        /// circumvent all the pattern matching by inheriting from a common static interface.
        template<typename Derived>
        struct IpAddr {
            // # Desctructor
            virtual ~IpAddr() = default;

            // # Public member functions

            /// Return `true` if IpAddr is set to `unspecified` address
            [[nodiscard]] constexpr bool is_unspecified() const noexcept {
                return static_cast<Derived*>(this)->is_unspecified();
            }

            /// Return `true` if IpAddr is set to a loopback address
            [[nodiscard]] constexpr bool is_loopback() const noexcept {
                return static_cast<Derived*>(this)->is_loopback();
            }

            /// Return `true` if IpAddr is within a globally routable range
            [[nodiscard]] constexpr bool is_global()  const noexcept {
                return static_cast<Derived*>(this)->is_global();
            }

            /// Return `true` if IpAddr is set to a multicast address
            [[nodiscard]] constexpr bool is_multicast() const noexcept {
                return static_cast<Derived*>(this)->is_multicast();
            }

            /// Return `true` if IpAddr is within a range designated for documentation
            [[nodiscard]] constexpr bool is_documentation() const noexcept {
                return static_cast<Derived*>(this)->is_documentation();
            }

            /// Returns `true` if IpAddr is implemented as an Ipv4Addr
            [[nodiscard]] constexpr bool is_ipv4() const noexcept {
                return static_cast<Derived*>(this)->is_ipv4();
            }

            /// Returns `true` if IpAddr is implemented as an Ipv6Addr
            [[nodiscard]] constexpr bool is_ipv6() const noexcept {
                return static_cast<Derived*>(this)->is_ipv6();
            }

        };

    } // namespace net::interface

} // namespace laio