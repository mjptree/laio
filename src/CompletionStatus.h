#ifndef COMPLETIONSTATUS_H
#define COMPLETIONSTATUS_H

#include "traits.h"
#include "Overlapped.h"

namespace laio {

    class CompletionStatus {
        OVERLAPPED_ENTRY _raw_overlapped_entry;
    public:
        explicit CompletionStatus(const OVERLAPPED_ENTRY& overlappedEntry) noexcept
            : _raw_overlapped_entry{overlappedEntry} {}

        explicit CompletionStatus(OVERLAPPED_ENTRY&& overlappedEntry) noexcept
            : _raw_overlapped_entry{std::move(overlappedEntry)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

        constexpr operator OVERLAPPED_ENTRY() const noexcept; // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

        /// Create new custom completion status
        static CompletionStatus create(unsigned long bytes, std::size_t token, Overlapped* overlapped) noexcept;

        /// Create new completion status from existing 'OVERLAPPED_ENTRY`
        static CompletionStatus from_entry(const OVERLAPPED_ENTRY& entry) noexcept;

        /// Create new zero-initialized completion status
        static CompletionStatus zero() noexcept;

        /// Returns the number of bytes that have been transferred in the I/O operation associated with this completion status
        unsigned long bytes_transferred() noexcept;

        /// Return token associated with the file handle whose I/O operation has completed
        std::size_t token() noexcept;

        /// Return pointer to the `OVERLAPPED` structure associated with the I/O operation related to this completion status
        OVERLAPPED* overlapped() noexcept;

        /// Return pointer to internal `OVERLAPPED_ENTRY`structure
        OVERLAPPED_ENTRY* entry() noexcept;
    };

    namespace trait {

        template<>
        constexpr bool send<CompletionStatus> = true;

        template<>
        constexpr bool sync<CompletionStatus> = true;

    } // namespace trait

} // namespace laio

#endif // COMPLETIONSTATUS_H
