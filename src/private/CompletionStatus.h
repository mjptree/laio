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
        explicit CompletionStatus(OVERLAPPED_ENTRY overlappedEntry)
                : _raw_overlapped_entry{overlappedEntry} {}

        explicit operator OVERLAPPED_ENTRY();

        explicit operator LPOVERLAPPED_ENTRY();
    };

} // namespace laio

#endif // COMPLETIONSTATUS_H
