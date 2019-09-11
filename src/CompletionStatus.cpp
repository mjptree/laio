#include <algorithm>
#include "CompletionStatus.h"

namespace laio {

    constexpr CompletionStatus::operator OVERLAPPED_ENTRY() const noexcept {
        return _raw_overlapped_entry;
    }

    CompletionStatus CompletionStatus::create(const unsigned long bytes, const std::size_t token, Overlapped* overlapped) noexcept {
        static_assert(sizeof token == sizeof(ULONG_PTR));
        return CompletionStatus{OVERLAPPED_ENTRY {
                static_cast<ULONG_PTR>(token),
                static_cast<LPOVERLAPPED>(*overlapped),
                0,
                static_cast<DWORD>(bytes),
        }};
    }

    CompletionStatus CompletionStatus::from_entry(const OVERLAPPED_ENTRY& entry) noexcept {
        return CompletionStatus{entry};
    }

    CompletionStatus CompletionStatus::zero() noexcept {
        return CompletionStatus::create(0, 0, nullptr);
    }

    unsigned long CompletionStatus::bytes_transferred() noexcept {
        return this->_raw_overlapped_entry.dwNumberOfBytesTransferred;
    }

    std::size_t CompletionStatus::token() noexcept {
        return static_cast<std::size_t>(this->_raw_overlapped_entry.lpCompletionKey);
    }

    OVERLAPPED* CompletionStatus::overlapped() noexcept {
        return this->_raw_overlapped_entry.lpOverlapped;
    }

    OVERLAPPED_ENTRY* CompletionStatus::entry() noexcept {
        return &this->_raw_overlapped_entry;
    }

} // namespace laio