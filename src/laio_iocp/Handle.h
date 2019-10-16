#ifndef LAIO_HANDLE_H
#define LAIO_HANDLE_H

#include <WinIncludes.h>

#include <handleapi.h>
#include <iostream>
#include <optional>
#include <variant>
#include <cstdint>
#include <win_error.h>
#include "Overload.h"

namespace laio {

    using std::uint8_t;

    template<typename T>
    using Result = std::variant<T, wse::win_error>;

    namespace iocp {

        class Handle {

            HANDLE _raw_handle;

            /// Public member functions
        public:

            explicit Handle(HANDLE handle) noexcept
                : _raw_handle{std::move(handle)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

            Handle(const Handle& other) noexcept = delete;

            Handle(Handle&& other) noexcept
                : _raw_handle{std::move(other._raw_handle)} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            {
                other._raw_handle = nullptr;
            }

            // TODO: Implement Rule-of-5

            /// Close windows I/O handle before clean-up
            ~Handle() noexcept {
                if (_raw_handle != nullptr) CloseHandle(_raw_handle);
            }

            /// Borrow raw windows I/O handle object
            HANDLE &raw() & noexcept {
                return _raw_handle;
            }

            /// Extract raw windows I/O handle object and consume wrapper
            HANDLE &&into_raw() && noexcept {
                HANDLE temp = std::move(_raw_handle); // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
                _raw_handle = nullptr;
                return std::move(temp); // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            }

            /// Synchronously write data to file or I/O device associated with this handle
            Result<std::size_t> write(gsl::span<const uint8_t> buf) noexcept {
                DWORD bytes = 0;

                // For unsigned char `buf.size_bytes() == buf.size()`
                const DWORD len = (std::min)(static_cast<unsigned int>(buf.size_bytes()),
                                             (std::numeric_limits<std::size_t>::max)());
                const BOOL res = WriteFile(
                        _raw_handle,
                        buf.data(),
                        len,
                        &bytes,
                        nullptr
                );
                if (res) {
                    return static_cast<std::size_t>(bytes);
                } else {
                    return wse::win_error{};
                }
            }

            /// Synchronously read data from file or I/O device associated with this handle
            Result<std::size_t> read(gsl::span<uint8_t> buf) noexcept {
                DWORD bytes = 0;

                // For unsigned char `buf.size_bytes() == buf.size()`
                const DWORD len = (std::min)(static_cast<unsigned int>(buf.size_bytes()),
                                             (std::numeric_limits<std::size_t>::max)());
                const BOOL res = ReadFile(
                        _raw_handle,
                        buf.data(),
                        len,
                        &bytes,
                        nullptr
                );
                if (res) {
                    return static_cast<std::size_t>(bytes);
                } else {
                    return wse::win_error{};
                }
            }

            /// Asynchronously read data from file or I/O device and return immediately
            Result<std::optional<std::size_t>> read_overlapped(gsl::span<uint8_t> buf,
                                                               OVERLAPPED *overlapped) noexcept {
                return _read_overlapped_helper(buf, overlapped, FALSE);
            }

            /// Asynchronously read data from file or I/O device and wait for completion
            Result<std::size_t> read_overlapped_wait(gsl::span<uint8_t> buf, OVERLAPPED *overlapped) noexcept {
                const Result<std::optional<std::size_t>> res = _read_overlapped_helper(buf, overlapped, TRUE);

                // Does not throw if it accesses a `std::nullopt`, but due to `wait == TRUE`, that would constitute a logic error.
                return std::visit(Overload{
                        [](const std::optional<std::size_t> &arg) -> Result<std::size_t> { return *arg; },
                        [](const wse::win_error &arg) -> Result<std::size_t> { return arg; },
                }, res);
            }

            /// Asynchronously write data to file or I/O device and return immediately
            Result<std::optional<std::size_t>> write_overlapped(gsl::span<const uint8_t> buf,
                                                                OVERLAPPED *overlapped) noexcept {
                return _write_overlapped_helper(buf, overlapped, FALSE);
            }

            /// Asynchronously write data to file or I/O device and wait for completion
            Result<std::size_t> write_overlapped_wait(gsl::span<const uint8_t> buf, OVERLAPPED *overlapped) noexcept {
                const Result<std::optional<std::size_t>> res = _write_overlapped_helper(buf, overlapped, TRUE);

                // Does not throw if it accesses a `std::nullopt`, but due to `wait == TRUE`, that would constitute a logic error.
                return std::visit(Overload{
                        [](const std::optional<std::size_t> &arg) -> Result<std::size_t> { return *arg; },
                        [](const wse::win_error &arg) -> Result<std::size_t> { return arg; },
                }, res);
            }

            /// Private helper methods
        private:

            /// Asynchronously read data from file or I/O device associated with this handle
            Result<std::optional<std::size_t>> _read_overlapped_helper(gsl::span<uint8_t> buf, OVERLAPPED *overlapped,
                                                                       BOOLEAN wait) noexcept {

                // For unsigned char `buf.size_bytes() == buf.size()`
                const DWORD len = (std::min)(static_cast<unsigned int>(buf.size_bytes()),
                                             (std::numeric_limits<std::size_t>::max)());
                BOOL res = ReadFile(
                        _raw_handle,
                        buf.data(),
                        len,
                        nullptr,
                        overlapped
                );
                if (res == 0) {
                    const auto err = static_cast<wse::win_errc>(GetLastError());
                    if (err != wse::win_errc::io_pending) {
                        return wse::win_error{err};
                    }
                }
                DWORD bytes = 0;
                res = GetOverlappedResult(
                        _raw_handle,
                        overlapped,
                        &bytes,
                        static_cast<BOOL>(wait)
                );
                if (res == 0) {
                    const auto err = static_cast<wse::win_errc>(GetLastError());

                    // If you mess with this condition, you magically convert a logic error into a runtime error
                    if (err == wse::win_errc::io_incomplete && wait == FALSE) {
                        return std::nullopt;
                    } else {
                        return wse::win_error{err};
                    }
                }
                return static_cast<std::size_t>(bytes);
            }

            /// Asynchronously write data to file or I/O device associated with this handle
            Result<std::optional<std::size_t>> _write_overlapped_helper(gsl::span<const uint8_t> buf,
                                                                        OVERLAPPED *overlapped, BOOLEAN wait) noexcept {

                // For unsigned char `buf.size_bytes() == buf.size()`
                const DWORD len = (std::min)(static_cast<unsigned int>(buf.size_bytes()),
                                             (std::numeric_limits<std::size_t>::max)());
                BOOL res = WriteFile(
                        _raw_handle,
                        buf.data(),
                        len,
                        nullptr,
                        overlapped
                );
                if (res == 0) {
                    const auto err = static_cast<wse::win_errc>(GetLastError());
                    if (err != wse::win_errc::io_pending) {
                        return wse::win_error{err};
                    }
                }
                DWORD bytes = 0;
                res = GetOverlappedResult(_raw_handle, overlapped, &bytes, static_cast<BOOL>(wait));
                if (res == 0) {
                    const auto err = static_cast<wse::win_errc>(GetLastError());
                    if (err == wse::win_errc::io_incomplete && wait == FALSE) {
                        return std::nullopt;
                    } else {
                        return wse::win_error{err};
                    }
                }
                return static_cast<std::size_t>(bytes);
            }

        };

    } // namespace iocp

} // namespace laio

#endif // HANDLE_H
