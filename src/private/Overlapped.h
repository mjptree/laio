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

namespace laio {

    class Overlapped {
        OVERLAPPED _raw_overlapped;
    public:
        explicit constexpr Overlapped(OVERLAPPED overlapped) noexcept
            : _raw_overlapped(overlapped) {}

        explicit constexpr Overlapped(LPOVERLAPPED overlapped) noexcept
            : _raw_overlapped(*overlapped) {}

        operator OVERLAPPED() const noexcept;

        operator LPOVERLAPPED() noexcept;
    };

} // namespace laio

#endif // OVERLAPPED_H
