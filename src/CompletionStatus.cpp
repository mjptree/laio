#include "CompletionStatus.h"

namespace laio {


    constexpr CompletionStatus::operator OVERLAPPED_ENTRY() noexcept {
        return _raw_overlapped_entry;
    }

    constexpr CompletionStatus::operator LPOVERLAPPED_ENTRY() noexcept {
        return &_raw_overlapped_entry;
    }
} // namespace laio