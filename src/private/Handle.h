#ifndef LAIO_HANDLE_H
#define LAIO_HANDLE_H

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

#include <handleapi.h>
#include <iostream>
#include <optional>
#include <variant>

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    class Handle {
        HANDLE _raw_handle;
    public:
        explicit Handle(HANDLE handle)
                : _raw_handle(handle) {}

        ~Handle();

        HANDLE& raw();

        HANDLE into_raw() &&;

        Result<std::size_t> write(const unsigned char buf[]);

        Result<std::size_t> read(unsigned char buf[]);

        Result<std::optional<std::size_t>> read_overlapped(unsigned char buf[], OVERLAPPED *overlapped);

        Result<std::size_t> read_overlapped_wait(unsigned char buf[], OVERLAPPED *overlapped);

        Result<std::optional<std::size_t>> read_overlapped_helper(unsigned char buf[], OVERLAPPED *overlapped, BOOLEAN wait);

        Result<std::optional<std::size_t>> write_overlapped(const unsigned char buf[],  OVERLAPPED *overlapped);

        Result<std::size_t> write_overlapped_wait(const unsigned char buf[], OVERLAPPED *overlapped);

        Result<std::optional<std::size_t>> write_overlapped_helper(const unsigned char buf[], OVERLAPPED *overlapped, BOOLEAN wait);
    };

} // namespace laio

#endif // HANDLE_H
