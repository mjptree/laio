#ifndef WSAEXTENSION_H
#define WSAEXTENSION_H

#include <atomic>
#include <guiddef.h>

namespace laio::net {

    class WsaExtension {

        GUID _guid;
        std::atomic<std::size_t> _value;

    };

} // namespace laio::net

#endif // WSAEXTENSION_H
