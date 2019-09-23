#ifndef IOSLICEMUT_H
#define IOSLICEMUT_H

#include <WinSock2.h>

#include <variant>
#include <gsl/span>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class IoSpanMut {
            WSABUF _raw_wsa_buffer;
        public:
            explicit constexpr IoSpanMut(const WSABUF& wsaBuffer) noexcept
                : _raw_wsa_buffer{wsaBuffer} {}

            explicit constexpr IoSpanMut(WSABUF&& wsaBuffer) noexcept
                : _raw_wsa_buffer{std::move(wsaBuffer)} {}  // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

            static inline IoSpanMut create(unsigned char buf[]) noexcept {
                static_assert(sizeof(*buf) <= (std::numeric_limits<ULONG>::max)());

                // Seriously? Casting an unsigned char to a signed char?
                // Well behaved for all 128 ASCII characters - Blows up for any value lesser than 0 or greater than 127
                return IoSpanMut{std::move(WSABUF{  // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
                    sizeof(*buf),
                    reinterpret_cast<CHAR*>(buf),
                })};
            }

            inline Result<std::monostate> advance(std::size_t n) noexcept {
                if (static_cast<std::size_t>(sizeof(_raw_wsa_buffer.len)) < n) {
                    return std::out_of_range("Advancing out of range");
                }
                _raw_wsa_buffer.len -= static_cast<ULONG>(n);
                _raw_wsa_buffer.buf += n;
                return std::monostate{};
            }

            inline gsl::span<const unsigned char> as_span() noexcept {
                return gsl::span<const unsigned char>{
                    reinterpret_cast<const unsigned char*>(_raw_wsa_buffer.buf),
                    static_cast<int>(_raw_wsa_buffer.len)
                };
            }

            inline gsl::span<unsigned char> as_mut_span() noexcept {
                return gsl::span<unsigned char>{
                    reinterpret_cast<unsigned char*>(_raw_wsa_buffer.buf),
                    static_cast<int>(_raw_wsa_buffer.len)
                };
            }
        };

    } // namespace net

} // namespace laio

#endif // IOSLICEMUT_H
