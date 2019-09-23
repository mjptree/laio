#ifndef OVERLAPPED_H
#define OVERLAPPED_H

#include "WinIncludes.h"

#include <minwinbase.h>
#include <variant>
#include <win_error.h>
#include "traits.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    class Overlapped {
        
        OVERLAPPED _raw_overlapped;
        
    public:
        
        explicit constexpr Overlapped(const OVERLAPPED& overlapped) noexcept
            : _raw_overlapped{overlapped} {}

        explicit constexpr Overlapped(OVERLAPPED&& overlapped) noexcept
            : _raw_overlapped{std::move(overlapped)} {}

        constexpr operator OVERLAPPED() const noexcept {
            return _raw_overlapped;
        }

        /// Create new zero-initialized overlapped structure
        static Overlapped zero() noexcept {
            return Overlapped{ OVERLAPPED {
                    0,
                    0,
                    {
                            {0, 0}
                    }
            }};
        }

        /// Create new overlapped structure initialized with `bManualReset` = FALSE
        static Result<Overlapped> initialize_with_autoreset_event() noexcept {
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

        /// Create new overlapped structure from existing `OVERLAPPED`
        static Overlapped from_raw(const OVERLAPPED& overlapped) noexcept {
            return Overlapped{std::move(overlapped)}; // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
        }

        /// Return pointer to inner raw `OVERLAPPED` structure
        OVERLAPPED* raw() noexcept {
            return &_raw_overlapped;
        }

        /// Set the offset inside this overlapped structure
        void set_offset(unsigned long long offset) noexcept {
            _raw_overlapped.Offset = static_cast<DWORD>(offset);
            _raw_overlapped.OffsetHigh = static_cast<DWORD>(offset >> 32u);
        }

        /// Return the offset inside this overlapped structure
        unsigned long long offset() noexcept {
            return static_cast<unsigned long long>(_raw_overlapped.Offset)
                   | (static_cast<unsigned long long>(_raw_overlapped.OffsetHigh) << 32u);
        }

        /// Set the `hEvent`inside this overlapped structure
        void set_event(HANDLE event) noexcept {
            _raw_overlapped.hEvent = event;
        }

        /// Return the `hEvent`inside this overlapped structure
        HANDLE& event() noexcept {
            return _raw_overlapped.hEvent;
        }
        
    };

    namespace trait {

        template<>
        constexpr bool is_send<Overlapped> = true;

        template<>
        constexpr bool is_sync<Overlapped> = true;

    } // namespace trait

} // namespace laio

#endif // OVERLAPPED_H
