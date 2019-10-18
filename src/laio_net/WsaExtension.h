#ifndef WSAEXTENSION_H
#define WSAEXTENSION_H

#include <guiddef.h>

#include <atomic>

namespace laio::net {

    class WsaExtension {

        GUID guid_;
        std::atomic_size_t value_;

    };

} // namespace laio::net

#endif // WSAEXTENSION_H
