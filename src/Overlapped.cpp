#include "Overlapped.h"

namespace laio {

    Overlapped::operator OVERLAPPED() const noexcept {
        return _raw_overlapped;
    }

    Overlapped::operator LPOVERLAPPED() noexcept {
        return &_raw_overlapped;
    }

} // namespace laio