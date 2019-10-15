#ifndef COMPLETIONSTATUS_H
#define COMPLETIONSTATUS_H

#include <algorithm>
#include "traits.h"
#include "Overlapped.h"

namespace laio {

    namespace iocp {

        class CompletionStatus {

            OVERLAPPED_ENTRY _raw_overlapped_entry;

        public:

            explicit CompletionStatus(const OVERLAPPED_ENTRY &overlappedEntry) noexcept
                    : _raw_overlapped_entry{overlappedEntry} {}

            explicit CompletionStatus(OVERLAPPED_ENTRY &&overlappedEntry) noexcept
                    : _raw_overlapped_entry{
                    std::move(overlappedEntry)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

            constexpr operator OVERLAPPED_ENTRY() const noexcept { // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                return _raw_overlapped_entry;
            }

            /// Create new custom completion status
            static CompletionStatus create(const uint32_t bytes, std::size_t token, Overlapped *overlapped) noexcept {
                static_assert(sizeof token == sizeof(ULONG_PTR));
                return CompletionStatus{OVERLAPPED_ENTRY{
                        static_cast<ULONG_PTR>(token),
                        overlapped->raw(),
                        0,
                        static_cast<DWORD>(bytes),
                }};
            }

            /// Create new completion status from existing 'OVERLAPPED_ENTRY`
            static CompletionStatus from_entry(const OVERLAPPED_ENTRY &entry) noexcept {
                return CompletionStatus{std::move(entry)}; // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            }

            /// Create new zero-initialized completion status
            static CompletionStatus zero() noexcept {
                return CompletionStatus::create(0, 0, nullptr);
            }

            /// Returns the number of bytes that have been transferred in the I/O operation associated with this completion status
            uint32_t bytes_transferred() noexcept {
                return static_cast<uint32_t>(_raw_overlapped_entry.dwNumberOfBytesTransferred);
            }

            /// Return token associated with the file handle whose I/O operation has completed
            std::size_t token() noexcept {
                return static_cast<std::size_t>(_raw_overlapped_entry.lpCompletionKey);
            }

            /// Return pointer to the `OVERLAPPED` structure associated with the I/O operation related to this completion status
            OVERLAPPED *overlapped() noexcept {
                return _raw_overlapped_entry.lpOverlapped;
            }

            /// Return pointer to internal `OVERLAPPED_ENTRY`structure
            OVERLAPPED_ENTRY *entry() noexcept {
                return &_raw_overlapped_entry;
            }

        };

    } // namespace iocp

    namespace trait {

        template<>
        constexpr bool is_send<iocp::CompletionStatus> = true;

        template<>
        constexpr bool is_sync<iocp::CompletionStatus> = true;

    } // namespace trait

} // namespace laio

#endif // COMPLETIONSTATUS_H
