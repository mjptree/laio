#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
#pragma ide diagnostic ignored "hicpp-move-const-arg"
#pragma once

#include <WS2tcpip.h>

#include <cstdint>
#include <optional>
#include <string>
#include <sstream>
#include <tuple>
#include <variant>

#include "fmt/format.h"
#include "win_error.h"

#include "IpAddr.h"
#include "Parser.h"

namespace laio {

    using std::uint_fast8_t;
    using std::uint8_t;
    using std::uint16_t;

    template<typename T>
    using Result = std::variant<T, wse::win_error>;

    namespace net {

        class Ipv4Addr;

        /// Available scope definitions of IPv6 multicast
        enum class Ipv6MulticastScope {
            InterfaceLocal,
            LinkLocal,
            RealmLocal,
            AdminLocal,
            SiteLocal,
            OrganizationLocal,
            Global
        };

        /// IPv6 address implementation
        ///
        /// \details IPv6 as a 128-bit integer represented through eight 16-bit segments. This implementation assumes
        // host system with little-endian byte order. The class contains a Windows `IN6_ADDR` structure that abstracts
        // the handling of endianness, but requires unsafely accessing inactive union variants.
        class Ipv6Addr final : public interface::IpAddr<Ipv6Addr> {

            IN6_ADDR raw_ipv6_address_{};     ///< Windows IpV6 address structure (unsafe union access!)

        public:
            // # Constructors
            constexpr Ipv6Addr() noexcept = default;

            constexpr Ipv6Addr(IN6_ADDR ipV6Address) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                : raw_ipv6_address_{std::move(ipV6Address)} {}

            explicit constexpr Ipv6Addr(uint8_t a, uint8_t b, uint8_t c, uint8_t d,
                                        uint8_t e, uint8_t f, uint8_t g, uint8_t h,
                                        uint8_t i, uint8_t j, uint8_t k, uint8_t l,
                                        uint8_t m, uint8_t n, uint8_t o, uint8_t p) noexcept
                : raw_ipv6_address_{ {a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p} /* union */} {}

            explicit constexpr Ipv6Addr(uint16_t a, uint16_t b, uint16_t c, uint16_t d,
                                        uint16_t e, uint16_t f, uint16_t g, uint16_t h) noexcept
                : raw_ipv6_address_{ {static_cast<uint8_t>(a >> 8u), static_cast<uint8_t>(a),
                                     static_cast<uint8_t>(b >> 8u), static_cast<uint8_t>(b),
                                     static_cast<uint8_t>(c >> 8u), static_cast<uint8_t>(c),
                                     static_cast<uint8_t>(d >> 8u), static_cast<uint8_t>(d),
                                     static_cast<uint8_t>(e >> 8u), static_cast<uint8_t>(e),
                                     static_cast<uint8_t>(f >> 8u), static_cast<uint8_t>(f),
                                     static_cast<uint8_t>(g >> 8u), static_cast<uint8_t>(g),
                                     static_cast<uint8_t>(h >> 8u), static_cast<uint8_t>(h)} /* union */ } {}

            // # Operator overloads
            constexpr operator IN6_ADDR() const noexcept { // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                return raw_ipv6_address_;
            }

            explicit operator std::string() const noexcept {
                auto [a, b, c, d, e, f, g, h] = segments();

                // IPv4 convertible addresses, that require special formatting are handled first
                if (!a && !b && !c && !d && !e && (!f || f == 0xff'ff)) {

                    // Special case [0, 0, 0, 0, 0, 0, 0, 0] -> `::`
                    if (!g && !h) {
                        return "::";
                    }

                    // Special case [0, 0, 0, 0, 0, 0, 0, 1] -> `::1`
                    else if (!g && h == 1) {
                        return "::1";
                    }

                    // IPv4-compatible [0, 0, 0, 0, 0, 0, g, h]
                    else if (!f) {
                        return fmt::format("::{}.{}.{}.{}",
                                static_cast<uint8_t>(g >> 8u), static_cast<uint8_t>(g),
                                static_cast<uint8_t>(h >> 8u), static_cast<uint8_t>(h));
                    }

                    // IPv4-mapped [0, 0, 0, 0, 0, 0xff'ff, g, h]
                    else {
                        return fmt::format("::ffff:{}.{}.{}.{}",
                                           static_cast<uint8_t>(g >> 8u), static_cast<uint8_t>(g),
                                           static_cast<uint8_t>(h >> 8u), static_cast<uint8_t>(h));

                    }
                } // if IPv4 compatible addresses

                // General formatting rules for IPv6 addresses:
                // The 16-bit segments are represented in hexadecimal notation, separated by a double-colon each. The
                // longest sequence of zero segments can be contracted from e.g. `:0000:0000:` to `::`. Only one
                // sequence can be contracted in this manner. Leading zeros are omitted.
                else {
                    uint_fast8_t zero_span_len = 0;
                    uint_fast8_t zero_span_at = 0;
                    uint_fast8_t cur_span_len = 0;
                    uint_fast8_t cur_span_at = 0;
                    uint16_t segments[] = {a, b, c, d, e, f, g, h};

                    // Determine the longest span of consecutive zero-segments
                    for (uint_fast8_t i = 0; i < 8; ++i) {
                        if (!segments[i]) {
                            if (!cur_span_len) {
                                cur_span_at = i;
                            }
                            ++cur_span_len;
                            if (cur_span_len > zero_span_len) {
                                zero_span_len = cur_span_len;
                                zero_span_at = cur_span_at;
                            }
                        }
                        else {
                            cur_span_len = 0;
                            cur_span_at = 0;
                        }
                    }

                    // Format leading and trailing non-zero segments, omitting them if they are empty
                    if (zero_span_len > 1) {
                        std::stringstream builder{};
                        uint_fast8_t i = 1;

                        // If leading subspan is empty, then skip
                        if (zero_span_at) {
                            builder << fmt::format("{:x}", segments[0]);
                            for (; i < zero_span_at; ++i) {
                                builder << fmt::format(":{:x}", segments[i]);
                            }
                        }
                        builder << "::";
                        i = zero_span_at + zero_span_len;

                        // If trailing subspan is empty, then skip
                        if (i < 8) {
                            builder << fmt::format("{:x}", segments[i++]);
                            for (; i < 8; ++i) {
                                builder << fmt::format(":{:x}", segments[i]);
                            }
                        }
                        return builder.str();
                    }

                    // If no span with more than one zero segment has been found, then no special rules apply other than
                    // omitting leading zeroes
                    else {
                        return fmt::format("{:x}:{:x}:{:x}:{:x}:{:x}:{:x}:{:x}:{:x}", a, b, c, d, e, f, g, h);
                    }
                }
            }

            bool operator==(const Ipv6Addr& rhs) const noexcept {
                for (uint_fast8_t i = 0; i < 8; ++i) {
                    if (raw_ipv6_address_.u.Word[i] != rhs.raw_ipv6_address_.u.Word[i])
                        return false;
                }
                return true;
            }

            bool operator<(const Ipv6Addr& rhs) const noexcept {
                return segments() < rhs.segments();
            }

            bool operator==(const Ipv4Addr& rhs) const noexcept {
                return false;
            }

            bool operator<(const Ipv4Addr& rhs) const noexcept {
                return false;
            }

            // # Public member functions

            /// Parse address from string
            ///
            /// \return IPv6 address structure
            /*static std::optional<Ipv6Addr> from(const std::string& ipv6Address) {
                uint_fast32_t buf[8] = {0};
                uint_fast8_t pos = 0, octet = 0;
                for (auto i = ipv4Address.begin(); i < ipv4Address.end(); ++i) {
                    if (*i >= '0' && *i <= '9' && pos < 3 && octet < 4) {
                        buf[octet] = buf[octet] * 10 + (*i - '0');
                        if (buf[octet] > 255) return std::nullopt;
                        ++pos;
                    } else if (*i == '.' && pos < 4 && octet < 3) {
                        pos = 0;
                        ++octet;
                    } else {
                        return std::nullopt;
                    }
                }
                return Ipv6Addr{
                        static_cast<uint8_t>(buf[0]),
                        static_cast<uint8_t>(buf[1]),
                        static_cast<uint8_t>(buf[2]),
                        static_cast<uint8_t>(buf[3])
                };
            }*/

            /// Extract address in eight 16-bit integer segments
            ///
            /// \details Address is returned as a tuple and can be unpacked into its components via structured binding.
            ///
            /// \return Tuple with eight 16-bit integers
            [[nodiscard]] constexpr std::tuple<uint16_t, uint16_t, uint16_t, uint16_t,
                                               uint16_t, uint16_t, uint16_t, uint16_t> segments() const noexcept {
                uint16_t a = raw_ipv6_address_.u.Word[0] << 8u | raw_ipv6_address_.u.Word[0] >> 8u; // NOLINT(hicpp-signed-bitwise)
                uint16_t b = raw_ipv6_address_.u.Word[1] << 8u | raw_ipv6_address_.u.Word[1] >> 8u; // NOLINT(hicpp-signed-bitwise)
                uint16_t c = raw_ipv6_address_.u.Word[2] << 8u | raw_ipv6_address_.u.Word[2] >> 8u; // NOLINT(hicpp-signed-bitwise)
                uint16_t d = raw_ipv6_address_.u.Word[3] << 8u | raw_ipv6_address_.u.Word[3] >> 8u; // NOLINT(hicpp-signed-bitwise)
                uint16_t e = raw_ipv6_address_.u.Word[4] << 8u | raw_ipv6_address_.u.Word[4] >> 8u; // NOLINT(hicpp-signed-bitwise)
                uint16_t f = raw_ipv6_address_.u.Word[5] << 8u | raw_ipv6_address_.u.Word[5] >> 8u; // NOLINT(hicpp-signed-bitwise)
                uint16_t g = raw_ipv6_address_.u.Word[6] << 8u | raw_ipv6_address_.u.Word[6] >> 8u; // NOLINT(hicpp-signed-bitwise)
                uint16_t h = raw_ipv6_address_.u.Word[7] << 8u | raw_ipv6_address_.u.Word[7] >> 8u; // NOLINT(hicpp-signed-bitwise)
                return {a, b, c, d, e, f, g, h};
            }

            /// Return `true` if this address is of the multicast address space
            ///
            /// \details For IPv6 the multicast address space is: `ff00::/8`
            [[nodiscard]] constexpr bool is_multicast() const noexcept {
                return (raw_ipv6_address_.u.Word[0] & 0x00'ffu) == 0x00'ffu;
            }

            /// Determine the address' multicast scope if the address is multicast
            ///
            /// \return Scope of the IPv6 address if the address is multicast, `std::nullopt` otherwise
            [[nodiscard]] std::optional<Ipv6MulticastScope> multicast_scope() const noexcept {
                if (is_multicast()) {
                    switch (raw_ipv6_address_.u.Word[0] & 0x0f'00u) {
                        case 0x01'00: return Ipv6MulticastScope::InterfaceLocal;
                        case 0x02'00: return Ipv6MulticastScope::LinkLocal;
                        case 0x03'00: return Ipv6MulticastScope::RealmLocal;
                        case 0x04'00: return Ipv6MulticastScope::AdminLocal;
                        case 0x05'00: return Ipv6MulticastScope::SiteLocal;
                        case 0x08'00: return Ipv6MulticastScope::OrganizationLocal;
                        case 0x0e'00: return Ipv6MulticastScope::Global;
                        default: return std::nullopt;
                    }
                }
                return std::nullopt;
            }

            /// Return `true` if this address is unspecified
            ///
            /// \details For IPv6 the unspecified address is: `::`
            [[nodiscard]] constexpr bool is_unspecified() const noexcept {
                for (uint16_t segment: raw_ipv6_address_.u.Word) {
                    if (segment != 0)
                        return false;
                }
                return true;
            }

            /// Return `true` if this address is a loopback address
            ///
            /// \details For IPv6 the loopback address is: `::1`
            [[nodiscard]] constexpr bool is_loopback() const noexcept {
                for (uint_fast8_t i = 0; i < 7; ++i) {
                    if (raw_ipv6_address_.u.Word[i] != 0)
                        return false;
                }
                return raw_ipv6_address_.u.Word[7] == 0x01'00;
            }

            /// Return `true` if this address is of the unique local address space
            ///
            /// \details For IPv6 the unique local address space is: `fc00::/7`
            [[nodiscard]] constexpr bool is_unique_local() const noexcept {
                return (raw_ipv6_address_.u.Word[0] & 0x00'feu) == 0x00'fcu;
            }

            /// Return `true` if this address is of the unicast link-local address space
            ///
            /// \details This method validates the address as strictly adhering to the format defined in IETF RFC 4291.
            /// For IPv6 the unicast link-local address is strictly defined as: `fe80::/64`
            [[nodiscard]] constexpr bool is_unicast_link_local_strict() const noexcept {
                return raw_ipv6_address_.u.Word[0] == 0x80'feu
                    && raw_ipv6_address_.u.Word[1] == 0
                    && raw_ipv6_address_.u.Word[2] == 0
                    && raw_ipv6_address_.u.Word[3] == 0;
            }

            /// Return `true` if this address is of the unicast link-local address space
            ///
            /// \details This method validates the address as unicast link-local according to common conception as of
            /// the format: `fe80::/10`
            [[nodiscard]] constexpr bool is_unicast_link_local() const noexcept {
                return (raw_ipv6_address_.u.Word[0] & 0xc0'ff) == 0x80'feu; // NOLINT(hicpp-signed-bitwise)
            }

            /// Return `true` if this address is of the unicast site-local address space
            ///
            /// \details For IPv6 the unicast site-local address space is: `fec0::/10`
            [[nodiscard]] constexpr bool is_unicast_site_local() const noexcept {
                return (raw_ipv6_address_.u.Word[0] & 0xc0'ff) == 0xc0'feu; // NOLINT(hicpp-signed-bitwise)
            }

            /// Return `true` if this address is of the range reserved for documentation
            ///
            /// \details For IPv6 this address space is: `2001:db8::/32`
            [[nodiscard]] constexpr bool is_documentation() const noexcept {
                return raw_ipv6_address_.u.Word[0] == 0x01'20u && raw_ipv6_address_.u.Word[1] == 0xb8'0du;
            }

            /// Return `true` if this address is a globally routable unicast address
            ///
            /// \details IPv6 unicast addresses are not globally routable if they are: loopback, link-local, unique
            /// local, unspecified, or of the address range reserved for documentation.
            [[nodiscard]] constexpr bool is_unicast_global() const noexcept {
                return !is_multicast()
                    && !is_loopback()
                    && !is_unicast_link_local()
                    && !is_unique_local()
                    && !is_unspecified()
                    && !is_documentation();
            }

            /// Return `true` if this address appears to be globally routable
            ///
            /// \details IPv6 addresses are not globally routable if they are: loopback, link-local or unique local
            /// unicast, interface-, link-, realm-, admin- or site-local multicast.
            [[nodiscard]] constexpr bool is_global() const noexcept {
                return multicast_scope() == Ipv6MulticastScope::Global || is_unicast_global();
            }

            /// Return `false`, as this IP address is implemented as IPv6
            [[nodiscard]] constexpr bool is_ipv4() const noexcept {
                    return false;
            }

            /// Return `true`, as this IP address is implemented as IPv6
            [[nodiscard]] constexpr bool is_ipv6() const noexcept {
                    return true;
            }

            /// Turn this IPv6 address into an IPv4 address, if possible
            ///
            /// \details Implementation of this function is in "Ipv4Addr.h".
            ///
            /// \return Ipv4Addr structure if this IPv6 address either IPv4-compatible or IPv4-mapped, `std::nullopt`
            /// otherwise.
            [[nodiscard]] std::optional<Ipv4Addr> to_ipv4() const noexcept;

        }; // class Ipv6Addr

        namespace ipv6 {

            const Ipv6Addr LOCALHOST{0, 0, 0, 0, 0, 0, 0, 1};   ///< IPv6 localhost address

            const Ipv6Addr UNSPECIFIED{0, 0, 0, 0, 0, 0, 0, 0}; ///< IPv6 unspecified address

        } // namespace ipv6

    } // namespace net

} // namespace laio

#pragma clang diagnostic pop