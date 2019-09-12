#ifndef LAIO_HANDLE_H
#define LAIO_HANDLE_H

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef __WINDEF_
#include <windef.h>
#endif

#ifndef _WINUSER_
#include <winuser.h>
#endif

#ifndef __RPC_H__
#include <rpc.h>
#endif

#include <handleapi.h>
#include <iostream>
#include <optional>
#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    class Handle {
        HANDLE _raw_handle;

        /// Asynchronously read data from file or I/O device associated with this handle
        Result<std::optional<std::size_t>> read_overlapped_helper(unsigned char buf[], OVERLAPPED *overlapped, BOOLEAN wait) noexcept;

        /// Asynchronously write data to file or I/O device associated with this handle
        Result<std::optional<std::size_t>> write_overlapped_helper(const unsigned char buf[], OVERLAPPED *overlapped, BOOLEAN wait) noexcept;
    public:
        explicit Handle(HANDLE handle) noexcept
            : _raw_handle(handle) {}

        explicit Handle(HANDLE&& handle) noexcept
            : _raw_handle(std::move(handle)) {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

        /// Close windows I/O handle before clean-up
        ~Handle() noexcept;

        /// Borrow raw windows I/O handle object
        HANDLE& raw() & noexcept;

        /// Extract raw windows I/O handle object and consume wrapper
        HANDLE&& into_raw() && noexcept;

        /// Synchronously write data to file or I/O device associated with this handle
        Result<std::size_t> write(const unsigned char buf[]) noexcept;

        /// Synchronously read data from file or I/O device associated with this handle
        Result<std::size_t> read(unsigned char buf[]) noexcept;

        /// Asynchronously read data from file or I/O device and return immediately
        Result<std::optional<std::size_t>> read_overlapped(unsigned char buf[], OVERLAPPED *overlapped) noexcept;

        /// Asynchronously read data from file or I/O device and wait for completion
        Result<std::size_t> read_overlapped_wait(unsigned char buf[], OVERLAPPED *overlapped) noexcept;

        /// Asynchronously write data to file or I/O device and return immediately
        Result<std::optional<std::size_t>> write_overlapped(const unsigned char buf[],  OVERLAPPED *overlapped) noexcept;

        /// Asynchronously write data to file or I/O device and wait for completion
        Result<std::size_t> write_overlapped_wait(const unsigned char buf[], OVERLAPPED *overlapped) noexcept;

        // TODO: - Implement assignment and move assignment operator overloads
        //       - Revisit available constructors, consider deleting copy constructor
    };

} // namespace laio

#endif // HANDLE_H
