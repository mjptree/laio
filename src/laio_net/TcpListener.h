#pragma once

#include <variant>

#include "TcpListenerExt.h"
#include "traits.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

    class TcpListener : public interface::TcpListenerExt {

//            ////////////////////////////////////////////////////////////////////////////////
//            // TCP listeners
//            ////////////////////////////////////////////////////////////////////////////////
//
//                        pub struct TcpListener {
//                            inner: Socket,
//                        }
//
//                                impl TcpListener {
//                            pub fn bind(addr: io::Result<&SocketAddr>) -> io::Result<TcpListener> {
//                                    let addr = addr?;
//
//                                    init();
//
//                                    let sock = Socket::new(addr, c::SOCK_STREAM)?;
//
//                                    // On platforms with Berkeley-derived sockets, this allows
//                                    // to quickly rebind a socket, without needing to wait for
//                                    // the OS to clean up the previous one.
//                                    if !cfg!(windows) {
//                                        setsockopt(&sock, c::SOL_SOCKET, c::SO_REUSEADDR,
//                                                1 as c_int)?;
//                                    }
//
//                                    // Bind our new socket
//                                    let (addrp, len) = addr.into_inner();
//                                    cvt(unsafe { c::bind(*sock.as_inner(), addrp, len as _) })?;
//
//                                    // Start listening
//                                    cvt(unsafe { c::listen(*sock.as_inner(), 128) })?;
//                                    Ok(TcpListener { inner: sock })
//                            }
//
//                            pub fn socket(&self) -> &Socket { &self.inner }
//
//                            pub fn into_socket(self) -> Socket { self.inner }
//
//                            pub fn socket_addr(&self) -> io::Result<SocketAddr> {
//                                    sockname(|buf, len| unsafe {
//                                            c::getsockname(*self.inner.as_inner(), buf, len)
//                                    })
//                            }
//
//                            pub fn accept(&self) -> io::Result<(TcpStream, SocketAddr)> {
//                                    let mut storage: c::sockaddr_storage = unsafe { mem::zeroed() };
//                                    let mut len = mem::size_of_val(&storage) as c::socklen_t;
//                                    let sock = self.inner.accept(&mut storage as *mut _ as *mut _,
//                                    &mut len)?;
//                                    let addr = sockaddr_to_addr(&storage, len as usize)?;
//                                    Ok((TcpStream { inner: sock, }, addr))
//                            }
//
//                            pub fn duplicate(&self) -> io::Result<TcpListener> {
//                                    self.inner.duplicate().map(|s| TcpListener { inner: s })
//                            }
//
//                            pub fn set_ttl(&self, ttl: u32) -> io::Result<()> {
//                                    setsockopt(&self.inner, c::IPPROTO_IP, c::IP_TTL, ttl as c_int)
//                            }
//
//                            pub fn ttl(&self) -> io::Result<u32> {
//                                    let raw: c_int = getsockopt(&self.inner, c::IPPROTO_IP, c::IP_TTL)?;
//                                    Ok(raw as u32)
//                            }
//
//                            pub fn set_only_v6(&self, only_v6: bool) -> io::Result<()> {
//                                    setsockopt(&self.inner, c::IPPROTO_IPV6, c::IPV6_V6ONLY, only_v6 as c_int)
//                            }
//
//                            pub fn only_v6(&self) -> io::Result<bool> {
//                                    let raw: c_int = getsockopt(&self.inner, c::IPPROTO_IPV6, c::IPV6_V6ONLY)?;
//                                    Ok(raw != 0)
//                            }
//
//                            pub fn take_error(&self) -> io::Result<Option<io::Error>> {
//                                    self.inner.take_error()
//                            }
//
//                            pub fn set_nonblocking(&self, nonblocking: bool) -> io::Result<()> {
//                                    self.inner.set_nonblocking(nonblocking)
//                            }
//                        }
//
//                        impl FromInner<Socket> for TcpListener {
//                            fn from_inner(socket: Socket) -> TcpListener {
//                                    TcpListener { inner: socket }
//                            }
//                        }
//
//                        impl fmt::Debug for TcpListener {
//                            fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
//                            let mut res = f.debug_struct("TcpListener");
//
//                            if let Ok(addr) = self.socket_addr() {
//                                res.field("addr", &addr);
//                            }
//
//                            let name = if cfg!(windows) {"socket"} else {"fd"};
//                            res.field(name, &self.inner.as_inner())
//                                    .finish()
//                        }
//                    }

        };

    } // namespace net

    namespace trait {

        template<>
        constexpr bool as_raw_socket<net::TcpListener> = true;

        template<>
        constexpr bool from_raw_socket<net::TcpListener> = true;

        template<>
        constexpr bool into_raw_socket<net::TcpListener> = true;

    } // namespace trait

} // namespace laio