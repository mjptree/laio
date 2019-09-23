#ifndef TCPSTREAM_H
#define TCPSTREAM_H

#include <variant>
#include "TcpStreamExt.h"
#include "traits.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class TcpStream : public interface::TcpStreamExt {

//            ////////////////////////////////////////////////////////////////////////////////
//            // TCP streams
//            ////////////////////////////////////////////////////////////////////////////////
//
//                        pub struct TcpStream {
//                            inner: Socket,
//                        }
//
//                                impl TcpStream {
//                            pub fn connect(addr: io::Result<&SocketAddr>) -> io::Result<TcpStream> {
//                                    let addr = addr?;
//
//                                    init();
//
//                                    let sock = Socket::new(addr, c::SOCK_STREAM)?;
//
//                                    let (addrp, len) = addr.into_inner();
//                                    cvt_r(|| unsafe { c::connect(*sock.as_inner(), addrp, len) })?;
//                                    Ok(TcpStream { inner: sock })
//                            }
//
//                            pub fn connect_timeout(addr: &SocketAddr, timeout: Duration) -> io::Result<TcpStream> {
//                                    init();
//
//                                    let sock = Socket::new(addr, c::SOCK_STREAM)?;
//                                    sock.connect_timeout(addr, timeout)?;
//                                    Ok(TcpStream { inner: sock })
//                            }
//
//                            pub fn socket(&self) -> &Socket { &self.inner }
//
//                            pub fn into_socket(self) -> Socket { self.inner }
//
//                            pub fn set_read_timeout(&self, dur: Option<Duration>) -> io::Result<()> {
//                                    self.inner.set_timeout(dur, c::SO_RCVTIMEO)
//                            }
//
//                            pub fn set_write_timeout(&self, dur: Option<Duration>) -> io::Result<()> {
//                                    self.inner.set_timeout(dur, c::SO_SNDTIMEO)
//                            }
//
//                            pub fn read_timeout(&self) -> io::Result<Option<Duration>> {
//                                    self.inner.timeout(c::SO_RCVTIMEO)
//                            }
//
//                            pub fn write_timeout(&self) -> io::Result<Option<Duration>> {
//                                    self.inner.timeout(c::SO_SNDTIMEO)
//                            }
//
//                            pub fn peek(&self, buf: &mut [u8]) -> io::Result<usize> {
//                                    self.inner.peek(buf)
//                            }
//
//                            pub fn read(&self, buf: &mut [u8]) -> io::Result<usize> {
//                                    self.inner.read(buf)
//                            }
//
//                            pub fn read_vectored(&self, bufs: &mut [IoSpanMut<'_>]) -> io::Result<usize> {
//                            self.inner.read_vectored(bufs)
//                        }
//
//                        pub fn write(&self, buf: &[u8]) -> io::Result<usize> {
//                            let len = cmp::min(buf.len(), <wrlen_t>::max_value() as usize) as wrlen_t;
//                            let ret = cvt(unsafe {
//                                    c::send(*self.inner.as_inner(),
//                                            buf.as_ptr() as *const c_void,
//                                    len,
//                                    MSG_NOSIGNAL)
//                            })?;
//                            Ok(ret as usize)
//                        }
//
//                        pub fn write_vectored(&self, bufs: &[IoSlice<'_>]) -> io::Result<usize> {
//                        self.inner.write_vectored(bufs)
//                    }
//
//                            pub fn peer_addr(&self) -> io::Result<SocketAddr> {
//                    sockname(|buf, len| unsafe {
//                    c::getpeername(*self.inner.as_inner(), buf, len)
//                })
//            }
//
//            pub fn socket_addr(&self) -> io::Result<SocketAddr> {
//            sockname(|buf, len| unsafe {
//            c::getsockname(*self.inner.as_inner(), buf, len)
//            })
//            }
//
//            pub fn shutdown(&self, how: Shutdown) -> io::Result<()> {
//            self.inner.shutdown(how)
//            }
//
//            pub fn duplicate(&self) -> io::Result<TcpStream> {
//            self.inner.duplicate().map(|s| TcpStream { inner: s })
//            }
//
//            pub fn set_nodelay(&self, nodelay: bool) -> io::Result<()> {
//            self.inner.set_nodelay(nodelay)
//            }
//
//            pub fn nodelay(&self) -> io::Result<bool> {
//            self.inner.nodelay()
//            }
//
//            pub fn set_ttl(&self, ttl: u32) -> io::Result<()> {
//            setsockopt(&self.inner, c::IPPROTO_IP, c::IP_TTL, ttl as c_int)
//            }
//
//            pub fn ttl(&self) -> io::Result<u32> {
//            let raw: c_int = getsockopt(&self.inner, c::IPPROTO_IP, c::IP_TTL)?;
//            Ok(raw as u32)
//            }
//
//            pub fn take_error(&self) -> io::Result<Option<io::Error>> {
//            self.inner.take_error()
//            }
//
//            pub fn set_nonblocking(&self, nonblocking: bool) -> io::Result<()> {
//            self.inner.set_nonblocking(nonblocking)
//            }
//            }
//
//            impl FromInner<Socket> for TcpStream {
//            fn from_inner(socket: Socket) -> TcpStream {
//            TcpStream { inner: socket }
//            }
//            }
//
//            impl fmt::Debug for TcpStream {
//            fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
//            let mut res = f.debug_struct("TcpStream");
//
//            if let Ok(addr) = self.socket_addr() {
//                res.field("addr", &addr);
//            }
//
//            if let Ok(peer) = self.peer_addr() {
//                res.field("peer", &peer);
//            }
//
//            let name = if cfg!(windows) {"socket"} else {"fd"};
//            res.field(name, &self.inner.as_inner())
//            .finish()
//            }
//            }

        };

    } // namespace net

    namespace trait {

        template<>
        constexpr bool as_raw_socket<net::TcpStream> = true;

        template<>
        constexpr bool from_raw_socket<net::TcpStream> = true;

        template<>
        constexpr bool into_raw_socket<net::TcpStream> = true;

    } // namespace trait

} // namespace laio

#endif // TCPSTREAM_H
