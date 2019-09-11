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

        static CompletionStatus create(unsigned long bytes, std::size_t token, Overlapped* overlapped) noexcept;

        static CompletionStatus from_entry(const OVERLAPPED_ENTRY& entry) noexcept;

        static CompletionStatus zero() noexcept;

        unsigned long bytes_transferred() noexcept;

        std::size_t token() noexcept;

        OVERLAPPED* overlapped() noexcept;

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
