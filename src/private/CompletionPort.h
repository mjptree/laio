#ifndef COMPLETIONPORT_H
#define COMPLETIONPORT_H

#include <chrono>
#include "Handle.h"
#include "CompletionStatus.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    class CompletionPort {
        Handle _handle;

        Result<std::monostate> _add(std::size_t token, HANDLE handle);
    public:
        explicit CompletionPort(const Handle& handle)
            : _handle(handle) {}

        static Result<CompletionPort> create(unsigned long threads);

        template<typename T>
        Result<std::monostate> add_handle(const std::size_t token, const T& t) {
            return this->_add(token, t.as_raw_handle());
        }

        template<typename T>
        Result<std::monostate> add_socket(const std::size_t token, const T& t) {
            return this->_add(token, static_cast<HANDLE>(t.as_raw_socket()));
        }

        Result<CompletionStatus> get(std::optional<const std::chrono::milliseconds> timeout);

        Result<CompletionStatus*> get_many(CompletionStatus list[], std::optional<const std::chrono::milliseconds> timeout);
    };

} // namespace laio

#endif // COMPLETIONPORT_H
