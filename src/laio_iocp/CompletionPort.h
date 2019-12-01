#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-move-const-arg"
#pragma once

#include <chrono>

// TODO: Replace by STL span as soon as available
#include "gsl/span"
#include "win_error.h"

#include "CompletionStatus.h"
#include "Handle.h"
#include "traits.h"

namespace laio {

    using std::uint_fast32_t;

    template<typename T>
    using Result = std::variant<T, wse::win_error>;

    namespace iocp {

        /// Handle to Windows I/O completion port
        ///
        /// \details Specialized wrapper around an ordinary Windows handle, but to a I/O completion port - It provides
        /// completion port specific operations and enforce ownership semantics.
        class CompletionPort {

            Handle handle_;    ///< Handle to CompletionPort

        public:
            // # Constructors
            explicit CompletionPort(Handle handle) noexcept
                : handle_{std::move(handle)} {}

            CompletionPort(const CompletionPort& other) = delete;

            CompletionPort(CompletionPort&& other) noexcept
                : handle_{std::move(other.handle_)} {}

            // # Operator overloads
            CompletionPort& operator=(const CompletionPort& rhs) = delete;

            CompletionPort& operator=(CompletionPort&& rhs) noexcept {
                handle_ = std::move(rhs.handle_);
                return *this;
            }

            // # Public member functions

            /// Create new windows I/O completion port with specified number of concurrent threads
            ///
            /// \param threads Supported concurrency value of the platform
            /// \return Variant with CompletionPort if successful, error type otherwise.
            static Result<CompletionPort> create(uint32_t threads) noexcept {
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

            /// Wrap raw handle to Windows I/O completion port
            ///
            /// \param handle Raw Windows handle.
            /// \return CompletionPort
            static CompletionPort from_raw_handle(HANDLE&& handle) noexcept {
                return CompletionPort{Handle{std::move(handle)}};
            }

            /// Borrow raw handle to windows I/O completion port
            ///
            /// \details Borrow HANDLE from CompletionPort. Callee retains ownership over handle and no manual clean-up
            /// is required!
            ///
            /// \return HANDLE Raw Windows handle to this completion port
            HANDLE as_raw_handle() noexcept {
                return handle_;
            }

            /// Extract raw handle to windows I/O completion port and consume wrapper
            ///
            /// \details Move HANDLE out of CompletionPort. Caller takes ownership over the HANDLE and must clean-up
            /// manually!
            ///
            /// \return HANDLE&& Raw Windows handle to this completion port
            HANDLE&& into_raw_handle() && noexcept {
                return std::move(handle_).into_raw();
            }

            /// Associate a windows I/O handle to this I/O completion port
            ///
            /// \details Take object, which is convertible into raw system I/O handle and add it to this CompletionPort.
            ///
            /// \param token Unique token
            /// \param t Type which implements the `as_raw_handle` trait
            /// \return Variant with error type in case the association of the handle failed
            template<typename T, typename = std::enable_if_t<trait::as_raw_handle<T>>>
            Result<std::monostate> add_handle(const std::size_t token, const T& t) const noexcept {
                return add_(token, t.as_raw_handle());
            }

            /// Associate a windows socket to this I/O completion port
            ///
            /// \details Take object, which is convertible into a raw socket and add it to this CompletionPort. The
            /// socket is treated as a raw I/O handle.
            ///
            /// \param token Unique token
            /// \param t Type which implements the `as_raw_socket` trait
            /// \return Variant with error type in case the association of the socket failed
            template<typename T, typename = std::enable_if_t<trait::as_raw_socket<T>>>
            Result<std::monostate> add_socket(const std::size_t token, const T &t) noexcept {
                return add_(token, static_cast<HANDLE>(t.as_raw_socket()));
            }


            /// Dequeue completion status from this I/O completion port
            ///
            /// \details Dequeue the next CompletionStatus at this port. If no status is queued, wait for the specified
            /// time before returning. If method is provided with `std::nullopt`, it will not time out and wait until an
            /// I/O operation is posted to the port. If timeout is zero, the function will return immediately, even if
            /// there is no operation to dequeue.
            ///
            /// \param timeout Time in milliseconds to wait for a completion status to become available at this port
            /// \return Variant with CompletionStatus if successful, error type otherwise
            Result<CompletionStatus> get(std::optional<const std::chrono::milliseconds> timeout) noexcept {
                DWORD bytes = 0;
                ULONG_PTR token = 0;
                LPOVERLAPPED overlapped = nullptr;
                const DWORD duration = timeout ? static_cast<DWORD>((*timeout).count()) : INFINITE;
                const BOOL ret = GetQueuedCompletionStatus(
                        handle_,
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
            ///
            /// \details Dequeue as many completion statuses as are currently queued and write them opaquely into the
            /// provided buffer of zeroed completion statuses.
            ///
            /// \param list Receiving buffer of zeroed completion statuses
            /// \param timeout Time in milliseconds to wait for completion statuses to become available at this port
            /// \return Variant with span over successfully dequeued CompletionStatus in the provided buffer, Exception
            /// otherwise
            Result<gsl::span<CompletionStatus>> get_many(gsl::span<CompletionStatus> list,
                                                         std::optional<const std::chrono::milliseconds> timeout) noexcept {

                // Make sure that when writing directly into the span the element widths properly align.
                static_assert(sizeof(CompletionStatus) == sizeof(OVERLAPPED_ENTRY));
                const ULONG len = (std::min)(static_cast<DWORD>(list.size()),
                        static_cast<DWORD>((std::numeric_limits<std::size_t>::max)()));
                ULONG removed = 0;
                const DWORD duration = timeout ? static_cast<DWORD>((*timeout).count()) : INFINITE;
                const BOOL ret = GetQueuedCompletionStatusEx(
                        handle_,
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
            ///
            /// \param status CompletionStatus to post to this completion port
            /// \return Variant with error type, in case the posting has failed
            Result<std::monostate> post(const CompletionStatus status) noexcept {
                const OVERLAPPED_ENTRY &overlappedEntry = status;
                const BOOL ret = PostQueuedCompletionStatus(
                        handle_,
                        overlappedEntry.dwNumberOfBytesTransferred,
                        overlappedEntry.lpCompletionKey,
                        overlappedEntry.lpOverlapped
                );
                if (ret == 0) {
                    return wse::win_error{};
                }
                return std::monostate{};
            }

        private:
            /// Associate a raw windows I/O handle to this I/O completion port
            ///
            /// \details Do all the heavy lifting.
            ///
            /// \param token Unique token
            /// \param handle Raw Windows I/O to associate with this completion port
            /// \return Variant with error type, in case the association has failed
            Result<std::monostate> add_(const std::size_t token, const HANDLE& handle) noexcept {

                // For 32-bit systems: int  = long = ptr = 32b
                // For 64-bit Unix   : long = ptr  = 64b and int = 32b
                // for 64-bit Windows: long long = ptr = 64b and int = long = 32b
                static_assert(sizeof token == sizeof(ULONG_PTR));
                HANDLE ret = CreateIoCompletionPort(
                        handle,
                        handle_,
                        static_cast<ULONG_PTR>(token),
                        0
                );
                if (ret == nullptr) {
                    return wse::win_error{};
                }

                // ret == this->handle_.raw()
                return std::monostate{};
            }

        }; // class CompletionPort

    } // namespace iocp

    namespace trait {

        template<>
        constexpr bool is_send<iocp::CompletionPort> = true;

        template<>
        constexpr bool is_sync<iocp::CompletionPort> = true;

        template<>
        constexpr bool as_raw_handle<iocp::CompletionPort> = true;

        template<>
        constexpr bool from_raw_handle<iocp::CompletionPort> = true;

        template<>
        constexpr bool into_raw_handle<iocp::CompletionPort> = true;

    } // namespace trait

} // namespace laio
