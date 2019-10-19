#pragma once

#include <WinSock2.h>

#include <chrono>
#include <mutex>
#include <variant>

#include "gsl/span"

#include "IoSpanMut.h"
#include "SocketAddr.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class Socket {
            SOCKET raw_socket_;
        public:
            explicit constexpr Socket(SOCKET socket) noexcept
                : raw_socket_{std::move(socket)} {} // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
                
            ~Socket() noexcept {
                
                // TODO: Handle consumption of the object through call to `into_raw_socket`
                closesocket(raw_socket_);
            }

            static void init() { // TODO: Consider changing return type to Result<std::monostate>
                static std::once_flag onceFlag;
                std::call_once(onceFlag, []{
                    WSADATA data{};
                    /* const DWORD ret = */ WSAStartup(0x202, &data); // Error code encoded in return value (0 for success)
                    std::atexit([]{
                        WSACleanup(); // Can fail - error can be retrieved with WSAGetLastError()
                    });
                });
            }

            static Result<Socket> create(const SocketAddr& address, const int type) noexcept {
                
            }

            Result<std::monostate> connect_timeout(const SocketAddr& addr, const std::chrono::nanoseconds timeout) noexcept;

            Result<Socket> accept(SOCKADDR* storage, int len) noexcept;

            Result<Socket> duplicate() noexcept;

            Result<std::size_t> recv_with_flags(gsl::span<unsigned char> buf, int flags) noexcept;

            Result<std::size_t> read(gsl::span<unsigned char> buf) noexcept;

            Result<std::size_t> read_vectored(gsl::span<IoSpanMut> buf) noexcept;
        };

    } // namespace net

} // namespace laio