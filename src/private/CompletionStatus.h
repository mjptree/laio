#ifndef COMPLETIONSTATUS_H
#define COMPLETIONSTATUS_H

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

    class CompletionStatus {
        OVERLAPPED_ENTRY _raw_overlapped_entry;
    public:
        explicit constexpr CompletionStatus(OVERLAPPED_ENTRY overlappedEntry) noexcept
                : _raw_overlapped_entry{overlappedEntry} {}

        explicit constexpr CompletionStatus(LPOVERLAPPED_ENTRY lpOverlappedEntry) noexcept
                : _raw_overlapped_entry{*lpOverlappedEntry} {}

        explicit constexpr operator OVERLAPPED_ENTRY() noexcept;

        explicit constexpr operator LPOVERLAPPED_ENTRY() noexcept;
    };

} // namespace laio

#endif // COMPLETIONSTATUS_H
