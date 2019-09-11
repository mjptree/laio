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

        /// Associates a raw windows I/O handle to this I/O completion port
        Result<std::monostate> _add(std::size_t token, const HANDLE& handle) noexcept;
    public:
        explicit CompletionPort(const Handle&& handle) noexcept
            : _handle{std::move(handle)} {}

        /// Create new windows I/O completion port with associated number of concurrent threads
        static Result<CompletionPort> create(unsigned long threads) noexcept;

        /// Wrap raw handle to windows I/O completion port
        static CompletionPort from_raw_handle(const HANDLE&& handle) noexcept;

        /// Borrow raw handle to windows I/O completion port
        HANDLE& as_raw_handle() & noexcept;

        /// Extract raw handle to windows I/O completion port and consume wrapper
        HANDLE&& into_raw_handle() && noexcept;

        /// Associates a windows I/O handle to this I/O completion port
        template<typename T, typename = std::enable_if_t<trait::as_raw_handle<T>> >
        Result<std::monostate> add_handle(const std::size_t token, const T& t) noexcept {
            return this->_add(token, t.as_raw_handle());
        }

        /// Associates a windows socket to this I/O completion port
        template<typename T, typename = std::enable_if_t<trait::as_raw_socket<T>> >
        Result<std::monostate> add_socket(const std::size_t token, const T& t) noexcept {
            return this->_add(token, static_cast<HANDLE>(t.as_raw_socket()));
        }


        /// Dequeue completion status from this I/O completion port
        Result<CompletionStatus> get(std::optional<const std::chrono::milliseconds> timeout) noexcept;

        /// Dequeue multiple completion statuses from this I/O completion port
        Result<gsl::span<CompletionStatus>> get_many(gsl::span<CompletionStatus> list, std::optional<const std::chrono::milliseconds> timeout) noexcept;

        /// Post a custom completion status to this I/O completion port
        Result<std::monostate> post(CompletionStatus status) noexcept;
    };

    namespace trait {

        template<>
        constexpr bool as_raw_handle<CompletionPort> = true;

        template<>
        constexpr bool from_raw_handle<CompletionPort> = true;

        template<>
        constexpr bool into_raw_handle<CompletionPort> = true;

    } // namespace trait

} // namespace laio

#endif // COMPLETIONPORT_H
