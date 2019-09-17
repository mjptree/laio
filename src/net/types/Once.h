#ifndef ONCE_H
#define ONCE_H

#include <atomic>
#include <variant>
#include <type_traits>
#include "traits.h"
#include "PoisonedException.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        constexpr std::size_t INCOMPLETE = 0x0;
        constexpr std::size_t POISONED = 0x1;
        constexpr std::size_t RUNNING = 0x2;
        constexpr std::size_t COMPLETE = 0x3;
        constexpr std::size_t STATE_MASK = 0x3;

        struct Once {
            std::atomic_size_t _state;

            constexpr static Once create() noexcept {
                return Once{std::atomic_size_t{INCOMPLETE}};
            }

            constexpr bool is_completed() noexcept {
                return _state.load(std::memory_order_acquire) == COMPLETE;
            }

            template<typename F, typename = std::enable_if_t<std::is_function<F>>
            inline Result<std::monostate> call_once(F callable) noexcept {
                if (this->is_completed()) return std::monostate;
                call_inner(false, callable)
            };

        private:
            template<typename F, typename = std::enable_if_t<std::is_function<F>>
            inline Result<std::monostate> call_inner(bool ignore_poisoning, F callable) noexcept {
                std::size_t state = _state.load(std::memory_order_seq_cst);
                while (true) {
                    switch (state) {
                        case COMPLETE: return std::monostate{};
                        case POISONED: if (!ignore_poisoning) return PoisonedException{"Instance has been previously poisoned"};
                        [[fallthrough]];
                        case POISONED | INCOMPLETE:
                    }
                }
            }
        };

    }

    namespace trait {

        template<>
        constexpr bool send<net::Once> = true;

        template<>
        constexpr bool sync<net::Once> = true;

    }

} // namespace laio::net

#endif // ONCE_H
