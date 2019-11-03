#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-move-const-arg"
#pragma ide diagnostic ignored "bugprone-branch-clone"
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
#pragma once

#include <WinSock2.h>

#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <variant>

#include "fmt/format.h"
#include "win_error.h"

#include "IpAddr.h"
#include "Ipv6Addr.h"

namespace laio {

    using std::uint8_t;
    using std::uint32_t;

    template<typename T>
    using Result = std::variant<T, wse::win_error>;

    namespace net {

        /// IPv4 address implementation
        ///
        /// \details IPv4 address as a 32-bit integer represented through four single bytes. This implementation assumes
        /// host system with little-endian byte order. The class contains a Windows `IN_ADDR` structure that abstracts
        /// the handling of endianness, but requires unsafely accessing inactive union variants.
        class Ipv4Addr final : public interface::IpAddr<Ipv4Addr> {

            IN_ADDR raw_ipv4_address_{};        ///< Windows IpV4 address structure (unsafe union access!)

        public:
            // # Constructors
            constexpr Ipv4Addr() noexcept = default;

            constexpr Ipv4Addr(IN_ADDR ipv4Address) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                : raw_ipv4_address_{std::move(ipv4Address)} {}

            explicit constexpr Ipv4Addr(const uint8_t a, const uint8_t b, const uint8_t c, const uint8_t d) noexcept
                : raw_ipv4_address_{ { {a, b, c, d} /* struct */ } /* union */ } {}

            explicit Ipv4Addr(const std::string& ipv4Address) noexcept {

            }

            // # Operator overloads
            constexpr operator IN_ADDR() const noexcept { // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                return raw_ipv4_address_;
            }

            explicit operator std::string() const noexcept {
                auto [a, b, c, d] = octets();
                return fmt::format("{}.{}.{}.{}", a, b, c, d);
            }

            bool operator==(const Ipv4Addr& rhs) const noexcept {
                return raw_ipv4_address_.S_un.S_addr == rhs.raw_ipv4_address_.S_un.S_addr;
            }

            bool operator<(const Ipv4Addr& rhs) const noexcept {
                auto [lhs_a, lhs_b, lhs_c, lhs_d] = octets();
                auto [rhs_a, rhs_b, rhs_c, rhs_d] = rhs.octets();
                return lhs_a <= rhs_b
                    && lhs_b <= rhs_b
                    && lhs_c <= rhs_c
                    && lhs_d < rhs_d;
            }

            bool operator==(const Ipv6Addr& rhs) const noexcept {
                return false;
            }

            bool operator<(const Ipv6Addr& rhs) const noexcept {
                return true;
            }

            // # Public member functions

            /// Extract address in four 8-bit integer format
            ///
            /// \details Address is returned as a tuple and can be unpacked into its components via structured binding.
            ///
            /// \return Tuple with four 8-bit integers
            [[nodiscard]] constexpr std::tuple<uint8_t, uint8_t, uint8_t,  uint8_t> octets() const noexcept {
                uint8_t a = raw_ipv4_address_.S_un.S_un_b.s_b1;
                uint8_t b = raw_ipv4_address_.S_un.S_un_b.s_b2;
                uint8_t c = raw_ipv4_address_.S_un.S_un_b.s_b3;
                uint8_t d = raw_ipv4_address_.S_un.S_un_b.s_b4;
                return {a, b, c, d};
            }

            /// Return `true` if this address is unspecified
            ///
            /// \details For IPv4 the unspecified address is: 0.0.0.0
            [[nodiscard]] constexpr bool is_unspecified() const noexcept {
                return raw_ipv4_address_.S_un.S_addr == 0x00'00'00'00;
            }

            /// Return `true` if this is a loopback address
            ///
            /// \details For IPv4 the loopback address space is: 127.0.0.0/8
            [[nodiscard]] constexpr bool is_loopback() const noexcept {
                return raw_ipv4_address_.S_un.S_un_b.s_b1 == 0x7f;
            }

            /// Return `true` if this is a private address
            ///
            /// \details For IPv4 the private address space includes: 10.0.0.0/8, 172.16.0.0/12, 192.168.0.0/16
            [[nodiscard]] constexpr bool is_private() const noexcept {

                // The last two bytes are not of interest
                auto [a, b, c_, d_] = octets();
                switch (a) {
                    case 10 : return true;
                    case 172: return b >= 16 && b <= 31;
                    case 192: return b == 168;
                    default : return false;
                }
            }

            /// Return `true` if this address is link-local
            ///
            /// \details For IPv4 the link-local address space is: 169.254.0.0/16
            [[nodiscard]] constexpr bool is_link_local() const noexcept {

                // The last two bytes are not of interest
                auto [a, b, c_, d_] = octets();
                return a == 169 && b == 254;
            }

            /// Return `true` if this address is the broadcast address
            ///
            /// \details For IPv4 the broadcast address is: 255.255.255.255
            [[nodiscard]] constexpr bool is_broadcast() const noexcept {
                return raw_ipv4_address_.S_un.S_addr == 0xff'ff'ff'ff;
            }

            /// Return `true` if this address is of the range designated for documentation
            ///
            /// \details For IPv4 the address space designated for documentation is: 192.0.2.0/24, 198.51.100.0/24,
            /// 203.0.113.0/24
            [[nodiscard]] constexpr bool is_documentation() const noexcept {
                switch (raw_ipv4_address_.S_un.S_addr & 0x00'ff'ff'ffu) {
                    case 0x00'02'00'c0: return true;
                    case 0x00'64'33'c6: return true;
                    case 0x00'71'00'cb: return true;
                    default: return false;
                }
            }

            /// Return `true` if this address is of the shared address space
            ///
            /// \details For IPv4 the shared address space is: 100.64.0.0/10
            [[nodiscard]] constexpr bool is_shared() const noexcept {
                return raw_ipv4_address_.S_un.S_un_b.s_b1 == 100
                    && (raw_ipv4_address_.S_un.S_un_b.s_b2 & 0b1100'0000u) == 0b0100'0000u;
            }

            /// Return `true` if this address is of the reserved range for IETF protocol assigment
            ///
            /// \details For IPv4 the address space reserved by the IANA for protocol assignment is: 192.0.0.0/24
            [[nodiscard]] constexpr bool is_ietf_protocol_assignment() const noexcept {
                auto [a, b, c, d_] = octets();
                return a == 192 && b == 0 && c == 0;
            }

            /// Return `true` if this address is of the range reserved for future use
            ///
            /// \details This address range may change in the future as new addresses are assigned by IANA. The address
            /// space currently reserved for future use is: 240.0.0.0/4
            [[nodiscard]] constexpr bool is_reserved() const noexcept {
                return (raw_ipv4_address_.S_un.S_un_b.s_b1 & 240u) == 240 && !is_broadcast();
            }

            /// Return `true` if this address is of the address space reserved for network device benchmarking
            ///
            /// \details For IPv4 the address space reserved for network device benchmarking is 198.18.0.0/15
            [[nodiscard]] constexpr bool is_benchmarking() const noexcept {
                return raw_ipv4_address_.S_un.S_un_b.s_b1 == 198
                       && (raw_ipv4_address_.S_un.S_un_b.s_b2 & 0xfeu) == 18;
            }

            /// Return `true` if this address appears to be globally routable
            ///
            /// \details IPv4 addresses are not globally routable if they are: private, loopback, link-local, broadcast,
            /// used for documentation, unspecified or from the 0.0.0.0/8 block, reserved for future protocol
            /// assignment (except 192.0.0.9/32 and 192.0.0.10/32), reserved for future use, or reserved for network
            /// devices benchmarking.
            [[nodiscard]] constexpr bool is_global() const noexcept {

                // Only 192.0.0.9 and 192.0.0.10 are globally routable in 192.0.0.0/24
                if (raw_ipv4_address_.S_un.S_addr == 0x09'00'00'c0 || raw_ipv4_address_.S_un.S_addr == 0x0a'00'00'c0)
                    return true;
                return !is_private()
                    && !is_loopback()
                    && !is_link_local()
                    && !is_broadcast()
                    && !is_documentation()
                    && !is_shared()
                    && !is_ietf_protocol_assignment()
                    && !is_reserved()
                    && !is_benchmarking()
                    && raw_ipv4_address_.S_un.S_un_b.s_b1 != 0;
            }

            /// Return `true` if this address is of the multicast address space
            ///
            /// \details For IPv4 the multicast address space is: 224.0.0.0/4
            [[nodiscard]] constexpr bool is_multicast() const noexcept {
                uint8_t a = raw_ipv4_address_.S_un.S_un_b.s_b1;
                return a >= 224 && a <= 239;
            }

            /// Convert this IPv4 address into an IPv4-compatible IPv6 address
            ///
            /// \return Ipv6Addr structure with address of the format `::a.b.c.d` for a given IPv4 address a.b.c.d
            [[nodiscard]] Ipv6Addr to_ipv6_compatible() const noexcept {
                auto [a, b, c, d] = octets();
                return Ipv6Addr{0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0, 0,
                                a, b, c, d};
            }

            /// Convert this IPv4 address into an IPv4-mapped IPv6 address
            ///
            /// \details Ipv6Addr structure with address of the format `::ffff:a.b.c.d` for a given IPv4 address a.b.c.d
            [[nodiscard]] Ipv6Addr to_ipv6_mapped() const noexcept {
                auto [a, b, c, d] = octets();
                return Ipv6Addr{0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0xFF, 0xFF,
                                a, b, c, d};
            }

            /// Return `true`, as this IP address is implemented as IPv4
            [[nodiscard]] constexpr bool is_ipv4() const noexcept {
                return true;
            }

            /// Return `false`, as this IP address is implemented as IPv4
            [[nodiscard]] constexpr bool is_ipv6() const noexcept {
                return false;
            }

        }; // class Ipv4Addr

        [[nodiscard]] std::optional<Ipv4Addr> Ipv6Addr::to_ipv4() const noexcept {
            auto [a, b, c, d, e, f, g, h] = segments();
            if (!a && !b && !c && !d && !e && (!f || f == 0xff'ff))
                return Ipv4Addr{static_cast<uint8_t>(g >> 8u),
                                static_cast<uint8_t>(g),
                                static_cast<uint8_t>(h >> 8u),
                                static_cast<uint8_t>(h)};
            return std::nullopt;
        }

        namespace ipv4 {

            const Ipv4Addr LOCALHOST{127, 0, 0, 1};         ///< IPv4 localhost address

            const Ipv4Addr UNSPECIFIED{0, 0, 0, 0};         ///< IPv4 unspecified address

            const Ipv4Addr BROADCAST{255, 255, 255, 255};   ///< IPv4 broadcast address

        } // namespace ipv4

    } // namespace net

} // namespace laio
#pragma clang diagnostic pop