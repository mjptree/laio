#ifndef OVERLAPPED_H
#define OVERLAPPED_H

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef __WINDEF_
#include <windef.h>
#endif

#ifndef _WINUSER_
#include <winuser.h>
#endif

#ifndef __RPC_H__
#include <rpc.h>
#endif

#include <minwinbase.h>
#include <variant>
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

        constexpr operator OVERLAPPED() const noexcept;

        /// Create new zero-initialized overlapped structure
        static Overlapped zero() noexcept;

        /// Create new overlapped structure initialized with `bManualReset` = FALSE
        static Result<Overlapped> initialize_with_autoreset_event() noexcept;

        /// Create new overlapped structure from existing `OVERLAPPED`
        static Overlapped from_raw(const OVERLAPPED& overlapped) noexcept;

        /// Return pointer to inner raw `OVERLAPPED` structure
        OVERLAPPED* raw() noexcept;

        /// Set the offset inside this overlapped structure
        void set_offset(unsigned long long offset) noexcept;

        /// Return the offset inside this overlapped structure
        unsigned long long offset() noexcept;

        /// Set the `hEvent`inside this overlapped structure
        void set_event(HANDLE event) noexcept;

        /// Return the `hEvent`inside this overlapped structure
        HANDLE& event() noexcept;
    };

    namespace trait {

        template<>
        constexpr bool send<Overlapped> = true;

        template<>
        constexpr bool sync<Overlapped> = true;

    } // namespace trait

} // namespace laio

#endif // OVERLAPPED_H
