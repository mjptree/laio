#pragma once

#include <WinIncludes.h>

#include <handleapi.h>

#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <variant>

#include "gsl/span"
#include "win_error.h"

#include "Overload.h"

namespace laio {

    using std::uint8_t;

    template<typename T>
    using Result = std::variant<T, wse::win_error>;

    namespace iocp {

        /// Generic handle to Windows system resources
        ///
        /// Member attributes:
        ///     HANDLE  : raw_handle_
        ///
        /// Traits:
        ///     is_send,
        ///     is_sync
        ///
        /// Wraps a raw Windows handle, which in general is an aliased pointer. This class provides generic read-write-
        /// functionality and enforces ownership semantics. It behaves in the widest sense similar to `std::unique_ptr`.
        class Handle {

            HANDLE raw_handle_;

        public:
            // # Constructors
            explicit Handle(HANDLE handle) noexcept
                : raw_handle_{std::move(handle)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

            Handle(const Handle& other) noexcept = delete;

            Handle(Handle&& other) noexcept
                : raw_handle_{std::move(other.raw_handle_)} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            {
                other.raw_handle_ = nullptr;
            }

            // # Destructor
            ~Handle() noexcept {
                // Close windows I/O handle before clean-up
                if (raw_handle_ != nullptr) CloseHandle(raw_handle_);
            }


            // # Operator overloads
            Handle& operator=(const Handle& rhs) = delete;

            Handle& operator=(Handle&& rhs) noexcept {
                raw_handle_ = std::move(rhs.raw_handle_); // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
                rhs.raw_handle_ = nullptr;
                return *this;
            }

            // # Public Member functions

            /// Borrow raw windows I/O handle object
            ///
            /// Parameters:
            ///     (none)
            ///
            /// Return value:
            ///     HANDLE&
            ///
            /// Returns a reference to its inner raw HANDLE, thus enables ownership through borrow-semantics. Logically,
            /// the lifetime of the reference is tied to the lifetime of the owner.
            /// Do not clean-up the HANDLE through the reference!
            HANDLE& raw() & noexcept {
                return raw_handle_;
            }

            /// Extract raw windows I/O handle object and consume wrapper
            ///
            /// Parameters:
            ///     (none)
            ///
            /// Return value:
            ///     HANDLE&&
            ///
            /// Offers similar behaviour to move assignment. `into_raw` allows to move the raw HANDLE out of the
            /// class. The wrapper is consumed in the process.
            /// The new owner takes the responsibility to clean-up after itself!
            HANDLE&& into_raw() && noexcept {
                HANDLE temp = std::move(raw_handle_); // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
                raw_handle_ = nullptr;
                return std::move(temp); // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            }

            /// Synchronously write data to file or I/O device associated with this handle
            ///
            /// Parameters:
            ///     gsl::span<const std::uint8_t>   : buf
            ///
            /// Return value:
            ///     Result<std::size_t>
            ///
            /// Writes from a provided output buffer this file handle. The buffer is borrowed as a non-owning view into
            /// the buffer. The write operation is performed sequentially. The buffer is required to provide an unsigned
            /// integer bytestream. Returns the number of bytes that were successfully written to the file.
            Result<std::size_t> write(gsl::span<const uint8_t> buf) noexcept {
                DWORD bytes = 0;

                // For unsigned char `buf.size_bytes() == buf.size()`
                const DWORD len = (std::min)(static_cast<DWORD>(buf.size_bytes()), static_cast<DWORD>((std::numeric_limits<std::size_t>::max)()));
                const BOOL res = WriteFile(
                        raw_handle_,
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
            ///
            /// Parameters:
            ///     gsl::span<std::uint8_t> : buf
            ///
            /// Return value:
            ///     Result<std::size_t>
            ///
            /// Reads from this file handle to a provided input buffer in blocking mode. The buffer is borrowed as a
            /// non-owning view into the buffer. The read operation is performed sequentially. The buffer is filled with
            /// an unsigned integer bytestream. Returns the number of bytes that were successfully read from the file.
            Result<std::size_t> read(gsl::span<uint8_t> buf) noexcept {
                DWORD bytes = 0;

                // For unsigned char `buf.size_bytes() == buf.size()`
                const DWORD len = (std::min)(static_cast<DWORD>(buf.size_bytes()), static_cast<DWORD>((std::numeric_limits<std::size_t>::max)()));
                const BOOL res = ReadFile(
                        raw_handle_,
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
            ///
            /// Parameters:
            ///     gsl::span<std::uint8_t> : buf,
            ///     OVERLAPPED*             : overlapped
            ///
            /// Return value:
            ///     Result<std::optional<std::size_t>>
            ///
            /// Submits a request to perform an overlapped read. The buffer is borrowed as a non-owning view into the
            /// buffer. The function returns immediately with the number of bytes that have already been read
            /// successfully by that time, if any.
            Result<std::optional<std::size_t>> read_overlapped(gsl::span<uint8_t> buf,
                                                               OVERLAPPED* overlapped) noexcept {
                return read_overlapped_helper_(buf, overlapped, FALSE);
            }

            /// Asynchronously read data from file or I/O device and wait for completion
            ///
            /// Parameters:
            ///     gsl::span<std::uint8_t> : buf,
            ///     OVERLAPPED*             : overlapped
            ///
            /// Return value:
            ///     Result<std::size_t>
            ///
            /// Submits a request to perform an overlapped read. The buffer is borrowed as a non-owning view into the
            /// buffer. Despite performing the read in non-blocking mode, the function will wait for the read operation
            /// to complete and then returns with the number of bytes that have successfully been read from this file.
            Result<std::size_t> read_overlapped_wait(gsl::span<uint8_t> buf, OVERLAPPED *overlapped) noexcept {
                const Result<std::optional<std::size_t>> res = read_overlapped_helper_(buf, overlapped, TRUE);

                // Does not throw if it accesses a `std::nullopt`, but due to `wait == TRUE`, that would constitute a logic error.
                return std::visit(Overload{
                        [](const std::optional<std::size_t> &arg) -> Result<std::size_t> { return *arg; },
                        [](const wse::win_error &arg) -> Result<std::size_t> { return arg; },
                }, res);
            }

            /// Asynchronously write data to file or I/O device and return immediately
            ///
            /// Parameters:
            ///     gsl::span<const std::uint8_t>   : buf,
            ///     OVERLAPPED*                     : overlapped
            ///
            /// Return value:
            ///     Result<std::optional<std::size_t>>
            ///
            /// Submits a request to perform an overlapped write. The buffer is borrowed as a non-owning view into the
            /// buffer. The function returns immediately with the number of bytes that have already been written
            /// successfully by that time, if any.
            Result<std::optional<std::size_t>> write_overlapped(gsl::span<const uint8_t> buf,
                                                                OVERLAPPED *overlapped) noexcept {
                return write_overlapped_helper_(buf, overlapped, FALSE);
            }

            /// Asynchronously write data to file or I/O device and wait for completion
            ///
            /// Parameters:
            ///     gsl::span<const std::uint8_t>   : buf,
            ///     OVERLAPPED*                     : overlapped
            ///
            /// Return value:
            ///     Result<std::size_t>
            ///
            /// Submits a request to perform an overlapped write. The buffer is borrowed as a non-owning view into the
            /// buffer. Despite performing the write in non-blocking mode, the function will wait for the write
            /// operation to complete and then returns with the number of bytes that have successfully been written to
            /// this file.
            Result<std::size_t> write_overlapped_wait(gsl::span<const uint8_t> buf, OVERLAPPED *overlapped) noexcept {
                const Result<std::optional<std::size_t>> res = write_overlapped_helper_(buf, overlapped, TRUE);

                // Does not throw if it accesses a `std::nullopt`, but due to `wait == TRUE`, that would constitute a logic error.
                return std::visit(Overload{
                        [](const std::optional<std::size_t> &arg) -> Result<std::size_t> { return *arg; },
                        [](const wse::win_error &arg) -> Result<std::size_t> { return arg; },
                }, res);
            }

        private:
            /// Asynchronously read data from file or I/O device associated with this handle
            ///
            /// Parameters:
            ///     gsl::span<std::uint8_t> : buf,
            ///     OVERLAPPED*             : overlapped,
            ///     BOOLEAN                 : wait
            ///
            /// Return value:
            ///     Result<std::optional<std::size_t>>
            ///
            /// Internally handles overlapped reads from this file handle. The method allows to specify whether to wait
            /// for the completion of the read operation or to return immediately. Returns the number of successfully
            /// read bytes by the time it returns, if any.
            Result<std::optional<std::size_t>> read_overlapped_helper_(gsl::span<uint8_t> buf, OVERLAPPED *overlapped,
                                                                       BOOLEAN wait) noexcept {

                // For unsigned char `buf.size_bytes() == buf.size()`
                const DWORD len = (std::min)(static_cast<DWORD>(buf.size_bytes()), static_cast<DWORD>((std::numeric_limits<std::size_t>::max)()));
                BOOL res = ReadFile(
                        raw_handle_,
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
                        raw_handle_,
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
            ///
            /// Parameters:
            ///     gsl::span<const std::uint8_t>   : buf,
            ///     OVERLAPPED*                     : overlapped,
            ///     BOOLEAN                         : wait
            ///
            /// Return value:
            ///     Result<std::optional<std::size_t>>
            ///
            /// Internally handles overlapped writes to this file handle. The method allows to specify whether to wait
            /// for the completion of the write operation or to return immediately. Returns the number of successfully
            //  written bytes by the time it returns, if any.
            Result<std::optional<std::size_t>> write_overlapped_helper_(gsl::span<const uint8_t> buf,
                                                                        OVERLAPPED *overlapped, BOOLEAN wait) noexcept {

                // For unsigned char `buf.size_bytes() == buf.size()`
                const DWORD len = (std::min)(static_cast<DWORD>(buf.size_bytes()), static_cast<DWORD>((std::numeric_limits<std::size_t>::max)()));
                BOOL res = WriteFile(
                        raw_handle_,
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
                res = GetOverlappedResult(raw_handle_, overlapped, &bytes, static_cast<BOOL>(wait));
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