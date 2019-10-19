#pragma once

#include "WinIncludes.h"

#include <minwinbase.h>

#include <cstdint>
#include <variant>

#include "win_error.h"

#include "traits.h"

namespace laio {

    using std::uint64_t;

    template<typename T>
    using Result = std::variant<T, wse::win_error>;

    namespace iocp {

        class Overlapped {

            OVERLAPPED raw_overlapped_{};

        public:
            constexpr Overlapped() noexcept = default;

            explicit constexpr Overlapped(OVERLAPPED overlapped) noexcept
                    : raw_overlapped_{std::move(overlapped)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

            constexpr operator OVERLAPPED() const noexcept { // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                return raw_overlapped_;
            }

            /// Create new zero-initialized overlapped structure
            static Overlapped zero() noexcept {
                return Overlapped{};
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
            static Overlapped from_raw(const OVERLAPPED &overlapped) noexcept {
                return Overlapped{std::move(overlapped)}; // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
            }

            /// Return pointer to inner raw `OVERLAPPED` structure
            OVERLAPPED *raw() noexcept {
                return &raw_overlapped_;
            }

            /// Set the offset inside this overlapped structure
            void set_offset(uint64_t offset) noexcept {
                raw_overlapped_.Offset = static_cast<DWORD>(offset);
                raw_overlapped_.OffsetHigh = static_cast<DWORD>(offset >> 32u);
            }

            /// Return the offset inside this overlapped structure
            uint64_t offset() noexcept {
                return static_cast<uint64_t>(raw_overlapped_.Offset)
                       | (static_cast<uint64_t>(raw_overlapped_.OffsetHigh) << 32u);
            }

            /// Set the `hEvent`inside this overlapped structure
            void set_event(HANDLE event) noexcept {
                raw_overlapped_.hEvent = event;
            }

            /// Return the `hEvent`inside this overlapped structure
            HANDLE &event() noexcept {
                return raw_overlapped_.hEvent;
            }

        };

    } // namespace iocp

    namespace trait {

        template<>
        constexpr bool is_send<iocp::Overlapped> = true;

        template<>
        constexpr bool is_sync<iocp::Overlapped> = true;

    } // namespace trait

} // namespace laio
