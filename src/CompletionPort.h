#ifndef COMPLETIONPORT_H
#define COMPLETIONPORT_H

#include <chrono>

// TODO: Replace by STL span as soon as available
#include <gsl/span>
#include "traits.h"
#include "Handle.h"
#include "CompletionStatus.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    class CompletionPort {
        Handle _handle;

        Result<std::monostate> _add(std::size_t token, const HANDLE& handle) noexcept;
    public:
        explicit CompletionPort(const Handle& handle) noexcept
            : _handle(handle) {}

        static Result<CompletionPort> create(unsigned long threads) noexcept;

        static CompletionPort from_raw_handle(HANDLE handle) noexcept;

        HANDLE as_raw_handle() noexcept;

        HANDLE into_raw_handle() && noexcept;

        template<typename T, typename = std::enable_if_t<trait::as_raw_handle<T>> >
        Result<std::monostate> add_handle(const std::size_t token, const T& t) noexcept {
            return this->_add(token, t.as_raw_handle());
        }

        template<typename T, typename = std::enable_if_t<trait::as_raw_socket<T>> >
        Result<std::monostate> add_socket(const std::size_t token, const T& t) noexcept {
            return this->_add(token, static_cast<HANDLE>(t.as_raw_socket()));
        }

        Result<CompletionStatus> get(std::optional<const std::chrono::milliseconds> timeout) noexcept;

        Result<gsl::span<CompletionStatus>> get_many(gsl::span<CompletionStatus> list, std::optional<const std::chrono::milliseconds> timeout) noexcept;

        Result<std::monostate> post(CompletionStatus status) noexcept;
    };

    template<>
    constexpr bool trait::as_raw_handle<CompletionPort> = true;

    template<>
    constexpr bool trait::from_raw_handle<CompletionPort> = true;

    template<>
    constexpr bool trait::into_raw_handle<CompletionPort> = true;

} // namespace laio

#endif // COMPLETIONPORT_H
