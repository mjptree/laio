#pragma once

#include <algorithm>

#include "Overlapped.h"
#include "traits.h"

namespace laio {

    namespace iocp {

        /// Status message received from I/O Completion Port
        ///
        /// Member attributes:
        ///     OVERLAPPED_ENTRY    : raw_overlapped_entry_
        ///
        /// Traits:
        ///     is_send,
        ///     is_sync
        ///
        /// Wraps a Windows `OVERLAPPED_ENTRY` structure. Usually read out of I/O Completion Ports, they can also be
        /// zero-initialized or custom created and posted to a completion port manually.
        class CompletionStatus {

            OVERLAPPED_ENTRY raw_overlapped_entry_{};

        public:
            // Constructors
            constexpr CompletionStatus() noexcept = default;

            explicit CompletionStatus(OVERLAPPED_ENTRY overlappedEntry) noexcept
                    : raw_overlapped_entry_{std::move(overlappedEntry)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

            constexpr operator OVERLAPPED_ENTRY() const noexcept { // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                return raw_overlapped_entry_;
            }

            // Public member functions

            /// Create new custom completion status
            ///
            /// Parameters:
            ///     const std::uint32_t : bytes,
            ///     std::size_t         : token,
            ///     Overlapped*         : overlapped
            ///
            /// Return value:
            ///     CompletionStatus
            ///
            /// A new Completion status must be provided with the number of bytes, that have been transferred in the
            /// I/O operation associated with this status message, the unique token to the file handle involved in the
            /// operation and a pointer to the corresponding overlapped structure.
            /// The integer arguments are upcasted from `int` to `long` if necessary, although this should generally
            /// not result in a widening of the type (or a narrowing when reading the value from the status) on
            /// Windows platforms.
            static CompletionStatus create(const uint32_t bytes, std::size_t token, Overlapped* overlapped) noexcept {
                return CompletionStatus{OVERLAPPED_ENTRY{
                        static_cast<ULONG_PTR>(token),
                        overlapped->raw(),
                        0,
                        static_cast<DWORD>(bytes),
                }};
            }

            /// Return the number of bytes transferred in the I/O operation associated with this completion status
            ///
            /// Parameters:
            ///     (none)
            ///
            /// Return value:
            ///     std::uint32_t
            ///
            /// The message contained in this status is the number of bytes that have been transferred in the I/O
            /// operation and is internally stored as a `long`, which is not guaranteed to match `std::uint32_t`,
            /// although on Windows that is generally the case.
            uint32_t bytes_transferred() noexcept {
                return static_cast<uint32_t>(raw_overlapped_entry_.dwNumberOfBytesTransferred);
            }

            /// Return token associated with the file handle whose I/O operation has completed
            ///
            /// Parameters:
            ///     (none)
            ///
            /// Return value:
            ///     std::size_t
            ///
            /// The token must uniquely identify the I/O handle that has been registered with a Completion Port and to
            /// which this status message belongs.
            std::size_t token() noexcept {
                return static_cast<std::size_t>(raw_overlapped_entry_.lpCompletionKey);
            }

            /// Return pointer to the `OVERLAPPED` structure associated with the I/O operation to this completion status
            ///
            /// Parameters:
            ///     (none)
            ///
            /// Return value:
            ///     OVERLAPPED*
            ///
            OVERLAPPED* overlapped() noexcept {
                return raw_overlapped_entry_.lpOverlapped;
            }

            /// Return pointer to internal `OVERLAPPED_ENTRY` structure
            ///
            /// Parameters:
            ///     (none)
            ///
            /// Return value:
            ///     OVERLAPPED_ENTRY*
            ///
            OVERLAPPED_ENTRY* entry() noexcept {
                return &raw_overlapped_entry_;
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