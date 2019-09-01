#include "CompletionStatus.h"

namespace laio {


    CompletionStatus::operator OVERLAPPED_ENTRY() {
        return _raw_overlapped_entry;
    }

    CompletionStatus::operator LPOVERLAPPED_ENTRY() {
        return &_raw_overlapped_entry;
    }
} // namespace laio