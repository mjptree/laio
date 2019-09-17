#ifndef POISONEDEXCEPTION_H
#define POISONEDEXCEPTION_H

#include <stdexcept>

namespace laio::net {

    class PoisonedException : public std::runtime_error {};

} // namespace laio::net

#endif // POISONEDEXCEPTION_H
