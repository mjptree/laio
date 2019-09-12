#include <algorithm>
#include "CompletionStatus.h"

namespace laio {

    constexpr CompletionStatus::operator OVERLAPPED_ENTRY() const noexcept {
        return _raw_overlapped_entry;
    }

    CompletionStatus CompletionStatus::create(const unsigned long bytes, const std::size_t token, Overlapped* overlapped) noexcept {
        static_assert(sizeof token == sizeof(ULONG_PTR));
        return CompletionStatus{ OVERLAPPED_ENTRY {
                static_cast<ULONG_PTR>(token),
                overlapped->raw(),
                0,
                static_cast<DWORD>(bytes),
        }};
    }

    CompletionStatus CompletionStatus::from_entry(const OVERLAPPED_ENTRY& entry) noexcept {
        return CompletionStatus{std::move(entry)}; // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
    }

    CompletionStatus CompletionStatus::zero() noexcept {
        return CompletionStatus::create(0, 0, nullptr);
    }

    unsigned long CompletionStatus::bytes_transferred() noexcept {
        return static_cast<unsigned long>(_raw_overlapped_entry.dwNumberOfBytesTransferred);
    }

    std::size_t CompletionStatus::token() noexcept {
        return static_cast<std::size_t>(_raw_overlapped_entry.lpCompletionKey);
    }

    OVERLAPPED* CompletionStatus::overlapped() noexcept {
        return _raw_overlapped_entry.lpOverlapped;
    }

    OVERLAPPED_ENTRY* CompletionStatus::entry() noexcept {
        return &_raw_overlapped_entry;
    }

} // namespace laio