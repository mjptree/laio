#include "catch2/catch.hpp"

#include "Ipv4Addr.h"
#include "Ipv6Addr.h"

TEST_CASE("Ipv4Addr") {
    using namespace laio::net;

    // Localhost address space: 127.0.0.0/8
    Ipv4Addr localhost{127, 0, 0, 1};
    Ipv4Addr not_localhost{45, 22, 13, 197};
    CHECK(localhost.is_loopback());
    CHECK(localhost == ipv4::LOCALHOST);
    CHECK_FALSE(not_localhost.is_loopback());
    CHECK_FALSE(not_localhost == ipv4::LOCALHOST);

    // Retrieve octets in correct order as a tuple
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> octets{127, 0, 0, 1};
    CHECK(localhost.octets() == octets);

    // Unspecified address: 0.0.0.0
    Ipv4Addr unspecified{0, 0, 0, 0};
    Ipv4Addr not_unspecified{45, 22, 13, 197};
    CHECK(unspecified.is_unspecified());
    CHECK(unspecified == ipv4::UNSPECIFIED);
    CHECK_FALSE(not_unspecified.is_unspecified());
    CHECK_FALSE(not_unspecified == ipv4::UNSPECIFIED);

    // Broadcast address: 255.255.255.255
    Ipv4Addr broadcast{255, 255, 255, 255};
    Ipv4Addr not_broadcast{236, 168, 10, 65};
    CHECK(broadcast.is_broadcast());
    CHECK(broadcast == ipv4::BROADCAST);
    CHECK_FALSE(not_broadcast.is_broadcast());
    CHECK_FALSE(not_broadcast == ipv4::BROADCAST);

    // Cast IPv4 addresss to a formatted string
    CHECK(std::string{localhost} == "127.0.0.1");
    CHECK(std::string{unspecified} == "0.0.0.0");
    CHECK(std::string{broadcast} == "255.255.255.255");
    CHECK_FALSE(std::string{localhost} == "45.22.13.197");
    CHECK_FALSE(std::string{broadcast} == "236.168.10.65");

    // Private address space: 10.0.0.0/8
    //                        172.16.0.0/12
    //                        192.168.0.0/16
    CHECK(Ipv4Addr{10, 0, 0, 1}.is_private());
    CHECK(Ipv4Addr{10, 10, 10, 10}.is_private());
    CHECK(Ipv4Addr{172, 16, 20, 10}.is_private());
    CHECK(Ipv4Addr{172, 29, 45, 14}.is_private());
    CHECK(Ipv4Addr{192, 168, 0, 2}.is_private());
    CHECK_FALSE(Ipv4Addr{172, 32, 0, 2}.is_private());
    CHECK_FALSE(Ipv4Addr{192, 169, 0, 2}.is_private());

    // Link-local address space: 169.254.0.0/16
    CHECK(Ipv4Addr{169, 254, 0, 0}.is_link_local());
    CHECK(Ipv4Addr{169, 254, 10, 65}.is_link_local());
    CHECK_FALSE(Ipv4Addr{16, 89, 10, 65}.is_link_local());

    // Globally routable addresses are...
    // ... not private
    CHECK_FALSE(Ipv4Addr{10, 254, 0, 0}.is_global());
    CHECK_FALSE(Ipv4Addr{192, 168, 10, 65}.is_global());
    CHECK_FALSE(Ipv4Addr{172, 16, 10, 65}.is_global());

    // ... not unspecified or 0.0.0.0/8
    CHECK_FALSE(Ipv4Addr{0, 1, 2, 3}.is_global());
    CHECK_FALSE(Ipv4Addr{0, 0, 0, 0}.is_global());
    CHECK_FALSE(ipv4::UNSPECIFIED.is_global());

    // ... not loopback
    CHECK_FALSE(Ipv4Addr{127, 0, 0, 1}.is_global());
    CHECK_FALSE(ipv4::LOCALHOST.is_global());

    // ... not broadcast
    CHECK_FALSE(Ipv4Addr{255, 255, 255, 255}.is_global());
    CHECK_FALSE(ipv4::BROADCAST.is_global());

    // ... not designated for documentation
    CHECK_FALSE(Ipv4Addr{192, 0, 2, 255}.is_global());
    CHECK_FALSE(Ipv4Addr{198, 51, 100, 65}.is_global());
    CHECK_FALSE(Ipv4Addr{203, 0, 113, 6}.is_global());

    // ... not shared
    CHECK_FALSE(Ipv4Addr{100, 100, 0, 0}.is_global());

    // ... not reserved for protocol assignment
    CHECK_FALSE(Ipv4Addr{192, 0, 0, 0}.is_global());
    CHECK_FALSE(Ipv4Addr{192, 0, 0, 255}.is_global());

    // ... not reserved for future use
    CHECK_FALSE(Ipv4Addr{250, 10, 20, 30}.is_global());

    // ... not reserved for network device benchmarking
    CHECK_FALSE(Ipv4Addr{198, 18, 0, 0}.is_global());

    // ... all other addresses
    CHECK(Ipv4Addr{1, 1, 1, 1}.is_global());
    CHECK(Ipv4Addr{80, 9, 12, 3}.is_global());
    CHECK(Ipv4Addr{192, 0, 0, 9}.is_global());
    CHECK(Ipv4Addr{192, 0, 0, 10}.is_global());

    // Shared address space: 100.64.0.0/10
    CHECK(Ipv4Addr{100, 64, 0, 0}.is_shared());
    CHECK(Ipv4Addr{100, 127, 255, 255}.is_shared());
    CHECK_FALSE(Ipv4Addr{100, 128, 0, 0}.is_shared());

    // Address space reserved by IANA for IETF protocol assignment: 192.0.0.0/24
    CHECK(Ipv4Addr{192, 0, 0, 0}.is_ietf_protocol_assignment());
    CHECK(Ipv4Addr{192, 0, 0, 8}.is_ietf_protocol_assignment());
    CHECK(Ipv4Addr{192, 0, 0, 9}.is_ietf_protocol_assignment());
    CHECK(Ipv4Addr{192, 0, 0, 255}.is_ietf_protocol_assignment());
    CHECK_FALSE(Ipv4Addr{192, 0, 1, 0}.is_ietf_protocol_assignment());
    CHECK_FALSE(Ipv4Addr{191, 255, 255, 255}.is_ietf_protocol_assignment());

    // Address space for network device benchmarking: 198.18.0.0/15
    CHECK_FALSE(Ipv4Addr{198, 17, 255, 255}.is_benchmarking());
    CHECK(Ipv4Addr{198, 18, 0, 0}.is_benchmarking());
    CHECK(Ipv4Addr{198, 19, 255, 255}.is_benchmarking());

    // Address space reserved by IANA for future use
    CHECK(Ipv4Addr{240, 0, 0, 0}.is_reserved());
    CHECK(Ipv4Addr{255, 255, 255, 254}.is_reserved());
    CHECK_FALSE(Ipv4Addr{239, 255, 255, 255}.is_reserved());
    CHECK_FALSE(Ipv4Addr{255, 255, 255, 255}.is_reserved());

    // Multicast address space: 224.0.0./4
    CHECK(Ipv4Addr{224, 254, 0, 0}.is_multicast());
    CHECK(Ipv4Addr{236, 168, 10, 65}.is_multicast());
    CHECK_FALSE(Ipv4Addr{172, 16, 10, 65}.is_multicast());

    // Address space designated for documentation: 192.0.2.0/24
    //                                             198.51.100.0/24
    //                                             203.0.113.0/24
    CHECK(Ipv4Addr{192, 0, 2, 255}.is_documentation());
    CHECK(Ipv4Addr{198, 51, 100, 65}.is_documentation());
    CHECK(Ipv4Addr{203, 0, 113, 6}.is_documentation());
    CHECK_FALSE(Ipv4Addr{193, 34, 17, 19}.is_documentation());

    // Convert IPv4 to an IPv4-compatible IPv6 address
    Ipv6Addr ipv4_compatible = Ipv4Addr{192, 0, 2, 255}.to_ipv6_compatible();
    CHECK(ipv4_compatible == Ipv6Addr{0, 0, 0, 0, 0, 0, 49152, 767});

    // Convert IPv4 to an IPv4-mapped IPv6 address
    Ipv6Addr ipv4_mapped = Ipv4Addr{192, 0, 2, 255}.to_ipv6_mapped();
    CHECK(ipv4_mapped == Ipv6Addr{0, 0, 0, 0, 0, 65535, 49152, 767});
}

TEST_CASE("Ipv6Addr") {
    using namespace laio::net;

    // Localhost address: `::1`
    Ipv6Addr localhost{0, 0, 0, 0, 0, 0, 0, 1};
    Ipv6Addr not_localhost{0, 0, 0, 0, 0, 0xff'ff, 0xc0'0a, 0x02'ff};
    CHECK(localhost.is_loopback());
    CHECK(localhost == ipv6::LOCALHOST);
    CHECK_FALSE(not_localhost.is_loopback());
    CHECK_FALSE(not_localhost == ipv6::LOCALHOST);

    // Unspecified address: `::`
    Ipv6Addr unspecified{0, 0, 0, 0, 0, 0, 0, 0};
    Ipv6Addr not_unspecified{0, 0, 0, 0, 0, 0xff'ff, 0xc0'0a, 0x02'ff};
    CHECK(unspecified.is_unspecified());
    CHECK(unspecified == ipv6::UNSPECIFIED);
    CHECK_FALSE(not_unspecified.is_unspecified());
    CHECK_FALSE(not_unspecified == ipv6::UNSPECIFIED);

    // Retrieve segments as a tuple
    Ipv6Addr mapped_ipv4 = Ipv4Addr{192, 0, 2, 255}.to_ipv6_mapped();
    std::tuple<uint16_t, uint16_t, uint16_t, uint16_t,
               uint16_t, uint16_t, uint16_t,  uint16_t> segments{0, 0, 0, 0, 0, 65535, 49152, 767};
    CHECK(mapped_ipv4.segments() == segments);

    // Globally routable addresses are...
    // ... not loopback
    CHECK_FALSE(Ipv6Addr{0, 0, 0, 0, 0, 0, 0, 1}.is_global());
    CHECK_FALSE(ipv6::LOCALHOST.is_global());

    // ... not link-local, site-local or unique local unicast
    CHECK_FALSE(Ipv6Addr{0xfe'80, 0, 0, 0, 0, 0, 0, 0}.is_global());
    /* CHECK_FALSE(Ipv6Addr{0xfe'c2, 0, 0, 0, 0, 0, 0, 0}.is_global());
     * Site-local unicast must not be supported in new implementations */
    CHECK_FALSE(Ipv6Addr{0xfc'02, 0, 0, 0, 0, 0, 0, 0}.is_global());

    // ... not interface-, link-, realm-, admin- or site-local multicast

    // ... all other addresses
    CHECK(Ipv6Addr{0, 0, 0, 0, 0, 0xff'ff, 0xc0'0a, 0x02'ff}.is_global());
    CHECK(Ipv6Addr{0, 0, 0x01'c9, 0, 0, 0xaf'c8, 0, 0x00'01}.is_global());

    // Unique local address space: `fc00::/7`
    CHECK(Ipv6Addr{0xfc'02, 0, 0, 0, 0, 0, 0, 0}.is_unique_local());
    CHECK_FALSE(Ipv6Addr{0, 0, 0, 0, 0, 0xff'ff, 0xc0'0a, 0x02'ff}.is_unique_local());

    // Unicast link-local address space RFC compliant: `fc80::/64`
    CHECK(Ipv6Addr{0xfe'80, 0, 0, 0, 0, 0, 0, 0}.is_unicast_link_local_strict());
    CHECK(Ipv6Addr{0xfe'80, 0, 0, 0, 0xff'ff, 0xff'ff, 0xff'ff, 0xff'ff}.is_unicast_link_local_strict());

    // Unicast link-local address space common: `fc80::/10`
    CHECK_FALSE(Ipv6Addr{0xfe'80, 0, 0, 1, 0, 0, 0, 0}.is_unicast_link_local_strict());
    CHECK(Ipv6Addr{0xfe'80, 0, 0, 1, 0, 0, 0, 0}.is_unicast_link_local());
    CHECK_FALSE(Ipv6Addr{0xfe'81, 0, 0, 0, 0, 0, 0, 0}.is_unicast_link_local_strict());
    CHECK(Ipv6Addr{0xfe'81, 0, 0, 0, 0, 0, 0, 0}.is_unicast_link_local());

    // Unicast site-local address space: `fec0::/10`
    CHECK(Ipv6Addr{0xfe'c2, 0, 0, 0, 0, 0, 0, 0}.is_unicast_site_local());
    CHECK_FALSE(Ipv6Addr{0, 0, 0, 0, 0, 0xff'ff, 0xc0'0a, 0x02'ff}.is_unicast_site_local());

    // Address space reserved for documentation: `2001:db8::/32`
    CHECK(Ipv6Addr{0x20'01, 0x0d'b8, 0, 0, 0, 0, 0, 0}.is_documentation());
    CHECK_FALSE(Ipv6Addr{0, 0, 0, 0, 0, 0xff'ff, 0xc0'0a, 0x02'ff}.is_documentation());

    // Globally routable unicast addresses are...
    // ... not loopback
    CHECK_FALSE(Ipv6Addr{0, 0, 0, 0, 0, 0, 0, 1}.is_unicast_global());
    CHECK_FALSE(ipv6::LOCALHOST.is_unicast_global());

    // ... not link-local or unique local
    CHECK_FALSE(Ipv6Addr{0xfe'80, 0, 0, 1, 0, 0, 0, 0}.is_unicast_global());
    CHECK_FALSE(Ipv6Addr{0xfc'02, 0, 0, 0, 0, 0, 0, 0}.is_unicast_global());

    // ... not unspecified
    CHECK_FALSE(Ipv6Addr{0, 0, 0, 0, 0, 0, 0, 0}.is_unicast_global());
    CHECK_FALSE(ipv6::UNSPECIFIED.is_unicast_global());

    // ... of the range reserved for documentation
    CHECK_FALSE(Ipv6Addr{0x20'01, 0x0d'b8, 0, 0, 0, 0, 0, 0}.is_unicast_global());

    // ... all other unicast addresses
    CHECK(Ipv6Addr{0, 0, 0, 0, 0, 0xff'ff, 0xc0'0a, 0x02'ff}.is_unicast_global());

    // Determine multicast scope of address, if address is multicast
    CHECK(Ipv6Addr{0xff'0e, 0, 0, 0, 0, 0, 0, 0}.multicast_scope() == Ipv6MulticastScope::Global);
    CHECK(Ipv6Addr{0, 0, 0, 0, 0, 0xff'ff, 0xc0'0a, 0x02'ff}.multicast_scope() == std::nullopt);

    // Multicast address space: `ff00::/8`
    CHECK(Ipv6Addr{0xff'00, 0, 0, 0, 0, 0, 0, 0}.is_multicast());
    CHECK_FALSE(Ipv6Addr{0, 0, 0, 0, 0, 0xff'ff, 0xc0'0a, 0x02'ff}.is_multicast());

    // Convert IPv6 to IPv4 address, if it is either IPv4-compatible or IPv4-mapped
    CHECK(Ipv6Addr{0xff'ff, 0, 0, 0, 0, 0, 0, 0}.to_ipv4() == std::nullopt);
    CHECK(Ipv6Addr{0, 0, 0, 0, 0, 0xff'ff, 0xc0'0a, 0x02'ff}.to_ipv4() == Ipv4Addr{192, 10, 2, 255});
    CHECK(Ipv6Addr{0, 0, 0, 0, 0, 0, 0, 1}.to_ipv4() == Ipv4Addr{0, 0, 0, 1});
}

TEST_CASE("Ipv4Addr string conversion") {
    using namespace laio::net;

    Ipv4Addr localhost = *Ipv4Addr::from("127.0.0.1");
    CHECK(localhost == ipv4::LOCALHOST);

    std::optional<Ipv4Addr> unspecified = Ipv4Addr::from("0.0.0.0");
    CHECK(unspecified == ipv4::UNSPECIFIED);

    Ipv4Addr broadcast = *Ipv4Addr::from("255.255.255.255");
    CHECK(broadcast == ipv4::BROADCAST);

    std::optional<Ipv4Addr> three_periods = Ipv4Addr::from("...");
    CHECK(three_periods == std::nullopt);

    std::optional<Ipv4Addr> letters = Ipv4Addr::from("...");
    CHECK(three_periods == std::nullopt);
}

TEST_CASE("Ipv6Addr string conversion") {
    using namespace laio::net;

    // IPv4-mapped address
    Ipv6Addr mapped = Ipv4Addr{192, 0, 2, 128}.to_ipv6_mapped();
    CHECK(std::string{mapped} == "::ffff:192.0.2.128");

    // IPv4-compatible address
    Ipv6Addr compatible = Ipv4Addr{192, 0, 2, 128}.to_ipv6_compatible();
    CHECK(std::string{compatible} == "::192.0.2.128");

    // Address with no zero segments
    Ipv6Addr no_zero{8, 9, 10, 11, 12, 13, 14, 15};
    CHECK(std::string{no_zero} == "8:9:a:b:c:d:e:f");

    // Multiple sequential zero segments
    Ipv6Addr multiple_zero{0xae, 0, 0, 0, 0, 0xff'ff, 0x01'02, 0x03'04};
    CHECK(std::string{multiple_zero} == "ae::ffff:102:304");

    // Single zero segment
    Ipv6Addr single_zero{1, 2, 3, 4, 5, 6, 0, 8};
    CHECK(std::string{single_zero} == "1:2:3:4:5:6:0:8");

    // Unspecified address
    CHECK(std::string{ipv6::UNSPECIFIED} == "::");

    // Loopback address
    CHECK(std::string{ipv6::LOCALHOST} == "::1");

    // Zero segments trailing
    Ipv6Addr trailing{1, 0, 0, 0, 0, 0, 0, 0};
    CHECK(std::string{trailing} == "1::");

    // Two multi-zero segments, where the second span is longer than the first
    Ipv6Addr unequal_ranges{1, 0, 0, 4, 0, 0, 0, 8};
    CHECK(std::string{unequal_ranges} == "1:0:0:4::8");

    // Two multi-zero segments of equal length
    Ipv6Addr equal_ranges{1, 0, 0, 4, 5, 0, 0, 8};
    CHECK(std::string{equal_ranges} == "1::4:5:0:0:8");
}