#include <algorithm>
#include <limits>
#include <win_error.h>
#include "CompletionPort.h"

namespace laio {

    Result<CompletionPort> CompletionPort::create(unsigned long threads) noexcept {

        // nullptr converts to NULL macro. Clang-tidy advises to use nullptr
        // in places, which would be set to NULL.
        HANDLE ret = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, static_cast<DWORD>(threads));
        if (ret == nullptr) {
            return wse::win_error{};
        }
        return CompletionPort{Handle{ret}};
    }

    Result<std::monostate> CompletionPort::_add(const std::size_t token, const HANDLE handle) noexcept {
        // TODO: To be implemented
        return {};
    }

    Result<CompletionStatus>
    CompletionPort::get(std::optional<const std::chrono::milliseconds> timeout) noexcept {
        DWORD bytes = 0;
        DWORD token = 0;
        LPOVERLAPPED* overlapped = nullptr;
        const DWORD duration = timeout ? static_cast<DWORD>((*timeout).count()) : INFINITE;
        const BOOL ret = GetQueuedCompletionStatus(this->_handle.raw(), &bytes, &token, overlapped, duration);
        if (ret == 0) {
            return wse::win_error{};
        }
        return CompletionStatus{OVERLAPPED_ENTRY {
            token,
            *overlapped,
            0,
            bytes,
        }};
    }

    Result<gsl::span<CompletionStatus>>
    CompletionPort::get_many(gsl::span<CompletionStatus> list, std::optional<const std::chrono::milliseconds> timeout) noexcept {

        // Make sure that when writing directly into the span the element widths properly align
        static_assert(sizeof(CompletionStatus) == sizeof(OVERLAPPED_ENTRY));
        const ULONG len = (std::min)(static_cast<unsigned int>(list.size_bytes()), (std::numeric_limits<std::size_t>::max)());
        ULONG removed = 0;
        const DWORD duration = timeout ? static_cast<DWORD>((*timeout).count()) : INFINITE;
        const BOOL ret = GetQueuedCompletionStatusEx(this->_handle.raw(), static_cast<LPOVERLAPPED_ENTRY>(*list.data()), len, &removed, duration, static_cast<BOOL>(FALSE));
        if (ret == 0) {
            return wse::win_error{};
        }

        // Return a non-owning view into the array, spanning only the successfully dequeued Completion Statuses
        return list.first(static_cast<std::size_t>(removed));
    }

} // namespace laio