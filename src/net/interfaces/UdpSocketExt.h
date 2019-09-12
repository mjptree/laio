#ifndef  UDPSOCKETEXT_H
#define  UDPSOCKETEXT_H

#include <variant>
#include <optional>
#include <tuple>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace interface {

        struct UdpSocketExt {

            virtual Result<std::optional<std::size_t>> recv_from_overlapped(unsigned char buf[], SocketAddrBuf* address, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> recv_overlapped(unsigned char buf[], OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> send_to_overlapped(const unsigned char buf[], SocketAddr& address, OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::optional<std::size_t>> send_overlapped(const unsigned char buf[], OVERLAPPED* overlapped) noexcept = 0;
            virtual Result<std::tuple<std::size_t, unsigned long>> result(OVERLAPPED* overlapped) noexcept = 0;
            // TODO: Set destructor to default (mark as virtual?)

        };

    } // namespace interface

} // namespace laio

#endif // UDPSOCKETEXT_H
