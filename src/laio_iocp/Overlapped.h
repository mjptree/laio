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

        /// Overlapped structure required for Windows Overlapped I/O
        ///
        /// Member attributes:
        ///     OVERLAPPED  : raw_overlapped_
        ///
        /// Traits:
        ///     is_send,
        ///     is_sync
        ///
        /// Wraps a raw Windows `OVERLAPPED` structure. This structure is provided alongside with I/O operations and
        /// contains required information about the mode of asynchronism.
        class Overlapped {

            OVERLAPPED raw_overlapped_{};

        public:
            // Constructors
            constexpr Overlapped() noexcept = default;

            explicit constexpr Overlapped(OVERLAPPED overlapped) noexcept
                    : raw_overlapped_{std::move(overlapped)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)

            constexpr operator OVERLAPPED() const noexcept { // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                return raw_overlapped_;
            }

            // Public member functions

            /// Create new overlapped structure initialized with `bManualReset` = FALSE
            ///
            /// Parameters:
            ///     (none)
            ///
            /// Return value:
            ///     Result<Overlapped>
            ///
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
                Overlapped overlapped{};
                overlapped.set_event(event);
                return overlapped;
            }

            /// Return pointer to inner raw `OVERLAPPED` structure
            ///
            /// Parameters:
            ///     (none)
            ///
            /// Return value:
            ///     OVERLAPPED*
            ///
            OVERLAPPED* raw() noexcept {
                return &raw_overlapped_;
            }

            /// Set the offset inside this overlapped structure
            ///
            /// Parameters:
            ///     std::uint64_t   : offset
            ///
            /// Return value:
            ///     (none)
            ///
            void set_offset(uint64_t offset) noexcept {
                raw_overlapped_.Offset = static_cast<DWORD>(offset);
                raw_overlapped_.OffsetHigh = static_cast<DWORD>(offset >> 32u);
            }

            /// Return the offset inside this overlapped structure
            ///
            /// Parameters:
            ///     (none)
            ///
            /// Return value:
            ///     std::uint64_t
            ///
            uint64_t offset() noexcept {
                return static_cast<uint64_t>(raw_overlapped_.Offset)
                       | (static_cast<uint64_t>(raw_overlapped_.OffsetHigh) << 32u);
            }

            /// Set the `hEvent`inside this overlapped structure
            ///
            /// Parameters:
            ///     HANDLE  : event
            ///
            /// Return value:
            ///     (none)
            ///
            void set_event(HANDLE event) noexcept {
                raw_overlapped_.hEvent = event;
            }

            /// Return the `hEvent`inside this overlapped structure
            ///
            /// Parameters:
            ///     (none)
            ///
            /// Return value:
            ///     HANDLE&
            ///
            HANDLE& event() noexcept {
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
