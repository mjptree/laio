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

    // Octets can be retrieved in correct order as a tuple
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

    // ... not local
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

    // Segments can be retrieved in as a tuple
    Ipv6Addr mapped_ipv4 = Ipv4Addr{192, 0, 2, 255}.to_ipv6_mapped();
    std::tuple<uint16_t, uint16_t, uint16_t, uint16_t,
               uint16_t, uint16_t, uint16_t,  uint16_t> segments{0, 0, 0, 0, 0, 65535, 49152, 767};
    CHECK(mapped_ipv4.segments() == segments);
}