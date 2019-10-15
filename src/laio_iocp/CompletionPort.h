#ifndef COMPLETIONPORT_H
#define COMPLETIONPORT_H

#include <chrono>

// TODO: Replace by STL span as soon as available
#include <gsl/span>
#include <win_error.h>
#include "traits.h"
#include "Handle.h"
#include "CompletionStatus.h"

namespace laio {

    using std::uint_fast32_t;

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace iocp {

        class CompletionPort {

            Handle _handle;

            /// Public member functions
        public:

            explicit CompletionPort(const Handle &&handle) noexcept
                    : _handle{std::move(handle)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

            /// Create new windows I/O completion port with associated number of concurrent threads
            static Result<CompletionPort> create(uint_fast32_t threads) noexcept {

                // nullptr converts to NULL macro. Clang-tidy advises to use nullptr in places, which would be set to NULL.
                HANDLE ret = CreateIoCompletionPort(
                        INVALID_HANDLE_VALUE,
                        nullptr,
                        0,
                        static_cast<DWORD>(threads)
                );
                if (ret == nullptr) {
                    return wse::win_error{};
                }
                return CompletionPort{Handle{ret}};
            }

            /// Wrap raw handle to windows I/O completion port
            static CompletionPort from_raw_handle(const HANDLE &&handle) noexcept {
                return CompletionPort{
                        Handle{std::move(handle)}}; // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            }

            /// Borrow raw handle to windows I/O completion port
            HANDLE &as_raw_handle() & noexcept {
                return _handle.raw();
            }

            /// Extract raw handle to windows I/O completion port and consume wrapper
            HANDLE &&into_raw_handle() && noexcept {
                return std::move(_handle).into_raw();
            }

            /// Associates a windows I/O handle to this I/O completion port
            template<typename T, typename = std::enable_if_t<trait::as_raw_handle<T>>>
            Result<std::monostate> add_handle(const std::size_t token, const T &t) noexcept {
                return _add(token, t.as_raw_handle());
            }

            /// Associates a windows socket to this I/O completion port
            template<typename T, typename = std::enable_if_t<trait::as_raw_socket<T>>>
            Result<std::monostate> add_socket(const std::size_t token, const T &t) noexcept {
                return _add(token, static_cast<HANDLE>(t.as_raw_socket()));
            }


            /// Dequeue completion status from this I/O completion port
            Result<CompletionStatus> get(std::optional<const std::chrono::milliseconds> timeout) noexcept {
                DWORD bytes = 0;
                DWORD token = 0;
                LPOVERLAPPED overlapped = nullptr;
                const DWORD duration = timeout ? static_cast<DWORD>((*timeout).count()) : INFINITE;
                const BOOL ret = GetQueuedCompletionStatus(
                        _handle.raw(),
                        &bytes,
                        &token,
                        &overlapped,
                        duration
                );
                if (ret == 0) {
                    return wse::win_error{};
                }
                return CompletionStatus{OVERLAPPED_ENTRY{
                        token,
                        overlapped,
                        0,
                        bytes,
                }};
            }

            /// Dequeue multiple completion statuses from this I/O completion port
            Result<gsl::span<CompletionStatus>> get_many(gsl::span<CompletionStatus> list,
                                                         std::optional<const std::chrono::milliseconds> timeout) noexcept {

                // Make sure that when writing directly into the span the element widths properly align.
                static_assert(sizeof(CompletionStatus) == sizeof(OVERLAPPED_ENTRY));
                const ULONG len = (std::min)(static_cast<unsigned int>(list.size()),
                                             (std::numeric_limits<std::size_t>::max)());
                ULONG removed = 0;
                const DWORD duration = timeout ? static_cast<DWORD>((*timeout).count()) : INFINITE;
                const BOOL ret = GetQueuedCompletionStatusEx(
                        _handle.raw(),
                        reinterpret_cast<LPOVERLAPPED_ENTRY>(list.data()),
                        len,
                        &removed,
                        duration,
                        static_cast<BOOL>(FALSE)
                );
                if (ret == 0) {
                    return wse::win_error{};
                }

                // Return a non-owning view into the array, spanning only the successfully dequeued Completion Statuses.
                return list.first(static_cast<std::size_t>(removed));
            }

            /// Post a custom completion status to this I/O completion port
            Result<std::monostate> post(const CompletionStatus status) noexcept {
                const OVERLAPPED_ENTRY &overlappedEntry = status;
                const BOOL ret = PostQueuedCompletionStatus(
                        _handle.raw(),
                        overlappedEntry.dwNumberOfBytesTransferred,
                        overlappedEntry.lpCompletionKey,
                        overlappedEntry.lpOverlapped
                );
                if (ret == 0) {
                    return wse::win_error{};
                }
                return std::monostate{};
            }

            /// Private helper methods
        private:

            /// Associates a raw windows I/O handle to this I/O completion port
            Result<std::monostate> _add(const std::size_t token, const HANDLE &handle) noexcept {

                // For 32-bit systems: int  = long = ptr = 32b
                // For 64-bit Unix   : long = ptr  = 64b and int = 32b
                // for 64-bit Windows: long long = ptr = 64b and int = long = 32b
                static_assert(sizeof token == sizeof(ULONG_PTR));
                HANDLE ret = CreateIoCompletionPort(
                        handle,
                        _handle.raw(),
                        static_cast<ULONG_PTR>(token),
                        0
                );
                if (ret == nullptr) {
                    return wse::win_error{};
                }

                // ret == this->_handle.raw()
                return std::monostate{};
            }

        };

    } // namespace iocp

    namespace trait {

        template<>
        constexpr bool as_raw_handle<iocp::CompletionPort> = true;

        template<>
        constexpr bool from_raw_handle<iocp::CompletionPort> = true;

        template<>
        constexpr bool into_raw_handle<iocp::CompletionPort> = true;

    } // namespace trait

} // namespace laio

#endif // COMPLETIONPORT_H
