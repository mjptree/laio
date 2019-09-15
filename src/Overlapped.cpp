#include "win_error.h"
#include "Overlapped.h"

namespace laio {

    constexpr Overlapped::operator OVERLAPPED() const noexcept {
        return _raw_overlapped;
    }

    Overlapped Overlapped::zero() noexcept {
        return Overlapped{ OVERLAPPED {
            0,
            0,
            {
                {0, 0}
            }
        }};
    }

    Result<Overlapped> Overlapped::initialize_with_autoreset_event() noexcept {
        HANDLE event = CreateEventW(
                nullptr,
                static_cast<BOOL>(0),
                static_cast<BOOL>(0),
                nullptr
                );
        if (event == nullptr) {
            return wse::win_error{};
        }
        Overlapped overlapped = Overlapped::zero();
        overlapped.set_event(event);
        return overlapped;
    }

    Overlapped Overlapped::from_raw(const OVERLAPPED& overlapped) noexcept {
        return Overlapped{std::move(overlapped)}; // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
    }

    OVERLAPPED* Overlapped::raw() noexcept {
        return &_raw_overlapped;
    }

    void Overlapped::set_offset(unsigned long long offset) noexcept {
        _raw_overlapped.Offset = static_cast<DWORD>(offset);
        _raw_overlapped.OffsetHigh = static_cast<DWORD>(offset >> 32u);
    }

    unsigned long long Overlapped::offset() noexcept {
        return static_cast<unsigned long long>(_raw_overlapped.Offset)
            | (static_cast<unsigned long long>(_raw_overlapped.OffsetHigh) << 32u);
    }

    void Overlapped::set_event(HANDLE event) noexcept {
        _raw_overlapped.hEvent = event;
    }

    HANDLE& Overlapped::event() noexcept {
        return _raw_overlapped.hEvent;
    }

} // namespace laio