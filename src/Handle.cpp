#include <algorithm>
#include <limits>
#include "win_error.h"
#include "overload.h"
#include "Handle.h"

namespace laio {

    Handle::~Handle() noexcept {

        // TODO: Determine, whether `CloseHandle` is not in fact taking care of invalid handle values itself
        if (_raw_handle != nullptr) CloseHandle(_raw_handle);
    }

    HANDLE& Handle::raw() noexcept {
        return _raw_handle;
    }

    HANDLE&& Handle::into_raw() && noexcept {
        HANDLE temp = std::move(_raw_handle); // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
        _raw_handle = nullptr;
        return std::move(temp); // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
    }

    Result<std::size_t> Handle::write(const unsigned char *buf) noexcept {
        DWORD bytes = 0;
        const DWORD len = (std::min)(sizeof buf, (std::numeric_limits<std::size_t>::max)());
        const BOOL res = WriteFile(
                this->_raw_handle,
                buf,
                len,
                &bytes,
                nullptr
                );
        if (res) {
            return static_cast<std::size_t>(bytes);
        } else {
            return wse::win_error{};
        }
    }

    Result<std::size_t> Handle::read(unsigned char *buf) noexcept {
        DWORD bytes = 0;
        const DWORD len = (std::min)(sizeof buf, (std::numeric_limits<std::size_t>::max)());
        const BOOL res = ReadFile(
                this->_raw_handle,
                buf,
                len,
                &bytes,
                nullptr
                );
        if (res) {
            return static_cast<std::size_t>(bytes);
        } else {
            return wse::win_error{};
        }
    }

    Result<std::optional<std::size_t>> Handle::read_overlapped(unsigned char *buf, OVERLAPPED *overlapped) noexcept {
        return this->read_overlapped_helper(buf, overlapped, FALSE);
    }

    Result<std::size_t> Handle::read_overlapped_wait(unsigned char *buf, OVERLAPPED *overlapped) noexcept {
        const Result<std::optional<std::size_t>> res = this->read_overlapped_helper(buf, overlapped, TRUE);

        // Technically throws if it accesses a `std::nullopt`, but due to `wait == TRUE`, that would constitute a logic error.
        return std::visit(overload {
            [] (const std::optional<std::size_t>& arg) -> Result<std::size_t> { return *arg; },
            [] (const std::exception& arg) -> Result<std::size_t> { return arg; },
            }, res);
    }

    Result<std::optional<std::size_t>>
    Handle::read_overlapped_helper(unsigned char *buf, OVERLAPPED *overlapped, BOOLEAN wait) noexcept {
        const DWORD len = (std::min)(sizeof buf, (std::numeric_limits<std::size_t>::max)());
        BOOL res = ReadFile(
                this->_raw_handle,
                buf,
                len,
                nullptr,
                overlapped
                );
        if (res == 0) {
            const auto err = static_cast<wse::win_errc>(GetLastError());
            if (err != wse::win_errc::io_pending) {
                return wse::win_error{err};
            }
        }
        DWORD bytes = 0;
        res = GetOverlappedResult(
                this->_raw_handle,
                overlapped,
                &bytes,
                static_cast<BOOL>(wait)
                );
        if (res == 0) {
            const auto err = static_cast<wse::win_errc>(GetLastError());

            // If you mess with this condition, you magically convert a logic error into a runtime error
            if (err == wse::win_errc::io_incomplete && wait == FALSE) {
                return std::nullopt;
            } else {
                return wse::win_error{err};
            }
        }
        return static_cast<std::size_t>(bytes);
    }

    Result<std::optional<std::size_t>> Handle::write_overlapped(const unsigned char *buf, OVERLAPPED *overlapped) noexcept {
        return this->write_overlapped_helper(buf, overlapped, FALSE);
    }

    Result<std::size_t> Handle::write_overlapped_wait(const unsigned char *buf, OVERLAPPED *overlapped) noexcept {
        const Result<std::optional<std::size_t>> res = this->write_overlapped_helper(buf, overlapped, TRUE);

        // Technically throws if it accesses a `std::nullopt`, but due to `wait == TRUE`, that would constitute a logic error.
        return std::visit(overload {
            [](const std::optional<std::size_t>& arg) -> Result<std::size_t> { return *arg; },
            [](const std::exception& arg) -> Result<std::size_t> { return arg; },
        }, res);
    }

    Result<std::optional<std::size_t>>
    Handle::write_overlapped_helper(const unsigned char *buf, OVERLAPPED *overlapped, BOOLEAN wait) noexcept {
        const DWORD len = (std::min)(sizeof buf, (std::numeric_limits<std::size_t>::max)());
        BOOL res = WriteFile(
                this->_raw_handle,
                buf,
                len,
                nullptr,
                overlapped
                );
        if (res == 0) {
            const auto err = static_cast<wse::win_errc>(GetLastError());
            if (err != wse::win_errc::io_pending) {
                return wse::win_error{err};
            }
        }
        DWORD bytes = 0;
        res = GetOverlappedResult(this->_raw_handle, overlapped, &bytes, static_cast<BOOL>(wait));
        if (res == 0) {
            const auto err = static_cast<wse::win_errc>(GetLastError());
            if (err == wse::win_errc::io_incomplete && wait == FALSE) {
                return std::nullopt;
            } else {
                return wse::win_error{err};
            }
        }
        return static_cast<std::size_t>(bytes);
    }

} // namespace laio