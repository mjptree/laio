#ifndef COMPLETIONSTATUS_H
#define COMPLETIONSTATUS_H

#include "traits.h"
#include "Overlapped.h"

namespace laio {

    class CompletionStatus {
        OVERLAPPED_ENTRY _raw_overlapped_entry;
    public:
        explicit constexpr CompletionStatus(const OVERLAPPED_ENTRY& overlappedEntry) noexcept
                : _raw_overlapped_entry{overlappedEntry} {}

        explicit constexpr CompletionStatus(const LPOVERLAPPED_ENTRY& lpOverlappedEntry) noexcept
                : _raw_overlapped_entry{*lpOverlappedEntry} {}

        constexpr operator OVERLAPPED_ENTRY() const noexcept;

        constexpr operator LPOVERLAPPED_ENTRY() noexcept;

        static CompletionStatus create(unsigned long bytes, std::size_t token, Overlapped* overlapped) noexcept;

        static CompletionStatus from_entry(const OVERLAPPED_ENTRY& entry) noexcept;

        static CompletionStatus zero() noexcept;

        unsigned long bytes_transferred() noexcept;

        std::size_t token() noexcept;

        OVERLAPPED* overlapped() noexcept;

        OVERLAPPED_ENTRY* entry() noexcept;
    };

    template<>
    constexpr bool send<CompletionStatus> = true;

    template<>
    constexpr bool sync<CompletionStatus> = true;

} // namespace laio

#endif // COMPLETIONSTATUS_H
