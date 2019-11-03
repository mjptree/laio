#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
#pragma ide diagnostic ignored "hicpp-move-const-arg"
#pragma once

#include <WS2tcpip.h>

#include <optional>
#include <variant>

#include "IpAddr.h"

namespace laio {

    using std::uint8_t;
    using std::uint16_t;

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class Ipv4Addr;

        ///
        ///
        /// \details
        enum class Ipv6MulticastScope {
            InterfaceLocal,
            LinkLocal,
            RealmLocal,
            AdminLocal,
            SiteLocal,
            OrganizationLocal,
            Global
        };

        ///
        ///
        /// \details
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

            bool operator==(const Ipv6Addr& rhs) const noexcept {
                for (uint_fast8_t i = 0; i < 8; ++i) {
                    if (raw_ipv6_address_.u.Word[i] != rhs.raw_ipv6_address_.u.Word[i])
                        return false;
                }
                return true;
            }

            // # Public member functions

            ///
            ///
            /// \details
            ///
            /// \return
            [[nodiscard]] constexpr std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t,  uint16_t> segments() const noexcept {
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

            ///
            ///
            /// \details
            ///
            /// \return
            [[nodiscard]] constexpr bool is_multicast() const noexcept {
                return (raw_ipv6_address_.u.Word[0] & 0xff'00u) == 0xff'00u;
            }

            ///
            ///
            /// \details
            ///
            /// \return
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

            ///
            ///
            /// \details
            ///
            /// \return
            [[nodiscard]] constexpr bool is_unspecified() const noexcept {
                for (uint16_t segment: raw_ipv6_address_.u.Word) {
                    if (segment != 0)
                        return false;
                }
                return true;
            }

            ///
            ///
            /// \details
            ///
            /// \return
            [[nodiscard]] constexpr bool is_loopback() const noexcept {
                for (uint_fast8_t i = 0; i < 7; ++i) {
                    if (raw_ipv6_address_.u.Word[i] != 0)
                        return false;
                }
                return raw_ipv6_address_.u.Word[7] == 0x01'00;
            }

            ///
            ///
            /// \details
            ///
            /// \return
            [[nodiscard]] constexpr bool is_unique_local() const noexcept {
                return (raw_ipv6_address_.u.Word[0] & 0x00'feu) == 0x00'fcu;
            }

            ///
            ///
            /// \details
            ///
            /// \return
            [[nodiscard]] constexpr bool is_unicast_link_local_strict() const noexcept {
                return raw_ipv6_address_.u.Word[0] == 0x80'feu
                    && raw_ipv6_address_.u.Word[1] == 0
                    && raw_ipv6_address_.u.Word[2] == 0
                    && raw_ipv6_address_.u.Word[3] == 0;
            }

            ///
            ///
            /// \details
            ///
            /// \return
            [[nodiscard]] constexpr bool is_unicast_link_local() const noexcept {
                return (raw_ipv6_address_.u.Word[0] & 0xc0'ff) == 0x80'feu;
            }

            ///
            ///
            /// \details
            ///
            /// \return
            [[nodiscard]] constexpr bool is_unicast_site_local() const noexcept {
                return (raw_ipv6_address_.u.Word[0] & 0xc0'ff) == 0xc0'feu;
            }

            ///
            ///
            /// \details
            ///
            /// \return
            [[nodiscard]] constexpr bool is_documentation() const noexcept {
                return raw_ipv6_address_.u.Word[0] == 0x01'20u && raw_ipv6_address_.u.Word[1] == 0xb8'0du;
            }

            ///
            ///
            /// \details
            ///
            /// \return
            [[nodiscard]] constexpr bool is_unicast_global() const noexcept {
                return !is_multicast()
                    && !is_loopback()
                    && !is_unicast_link_local()
                    && !is_unique_local()
                    && !is_unspecified()
                    && !is_documentation();
            }

            ///
            ///
            /// \details
            ///
            /// \return
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

            std::optional<Ipv4Addr> to_ipv4() const noexcept;

        };

        namespace ipv6 {

            const Ipv6Addr LOCALHOST{0, 0, 0, 0, 0, 0, 0, 1};   ///< IPv6 loopback address

            const Ipv6Addr UNSPECIFIED{0, 0, 0, 0, 0, 0, 0, 0}; ///< IPv6 unspecified address

        } // namespace ipv6

    } // namespace net

} // namespace laio

#pragma clang diagnostic pop