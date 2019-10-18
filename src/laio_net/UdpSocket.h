#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <WinSock2.h>

#include <variant>

#include "traits.h"
#include "UdpSocketExt.h"

namespace laio {

    template<typename T>
    using Result = std::variant<T, std::exception>;

    namespace net {

        class UdpSocket : public interface::UdpSocketExt {

//            ////////////////////////////////////////////////////////////////////////////////
//            // UDP
//            ////////////////////////////////////////////////////////////////////////////////
//
//                        pub struct UdpSocket {
//                            inner: Socket,
//                        }
//
//                                impl UdpSocket {
//                            pub fn bind(addr: io::Result<&SocketAddr>) -> io::Result<UdpSocket> {
//                                    let addr = addr?;
//
//                                    init();
//
//                                    let sock = Socket::new(addr, c::SOCK_DGRAM)?;
//                                    let (addrp, len) = addr.into_inner();
//                                    cvt(unsafe { c::bind(*sock.as_inner(), addrp, len as _) })?;
//                                    Ok(UdpSocket { inner: sock })
//                            }
//
//                            pub fn socket(&self) -> &Socket { &self.inner }
//
//                            pub fn into_socket(self) -> Socket { self.inner }
//
//                            pub fn peer_addr(&self) -> io::Result<SocketAddr> {
//                                    sockname(|buf, len| unsafe {
//                                            c::getpeername(*self.inner.as_inner(), buf, len)
//                                    })
//                            }
//
//                            pub fn socket_addr(&self) -> io::Result<SocketAddr> {
//                                    sockname(|buf, len| unsafe {
//                                            c::getsockname(*self.inner.as_inner(), buf, len)
//                                    })
//                            }
//
//                            pub fn recv_from(&self, buf: &mut [u8]) -> io::Result<(usize, SocketAddr)> {
//                                    self.inner.recv_from(buf)
//                            }
//
//                            pub fn peek_from(&self, buf: &mut [u8]) -> io::Result<(usize, SocketAddr)> {
//                                    self.inner.peek_from(buf)
//                            }
//
//                            pub fn send_to(&self, buf: &[u8], dst: &SocketAddr) -> io::Result<usize> {
//                                    let len = cmp::min(buf.len(), <wrlen_t>::max_value() as usize) as wrlen_t;
//                                    let (dstp, dstlen) = dst.into_inner();
//                                    let ret = cvt(unsafe {
//                                        c::sendto(*self.inner.as_inner(),
//                                                  buf.as_ptr() as *const c_void, len,
//                                        MSG_NOSIGNAL, dstp, dstlen)
//                                    })?;
//                                    Ok(ret as usize)
//                            }
//
//                            pub fn duplicate(&self) -> io::Result<UdpSocket> {
//                                    self.inner.duplicate().map(|s| UdpSocket { inner: s })
//                            }
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
//                            pub fn set_broadcast(&self, broadcast: bool) -> io::Result<()> {
//                                    setsockopt(&self.inner, c::SOL_SOCKET, c::SO_BROADCAST, broadcast as c_int)
//                            }
//
//                            pub fn broadcast(&self) -> io::Result<bool> {
//                                    let raw: c_int = getsockopt(&self.inner, c::SOL_SOCKET, c::SO_BROADCAST)?;
//                                    Ok(raw != 0)
//                            }
//
//                            pub fn set_multicast_loop_v4(&self, multicast_loop_v4: bool) -> io::Result<()> {
//                                    setsockopt(&self.inner, c::IPPROTO_IP, c::IP_MULTICAST_LOOP, multicast_loop_v4 as c_int)
//                            }
//
//                            pub fn multicast_loop_v4(&self) -> io::Result<bool> {
//                                    let raw: c_int = getsockopt(&self.inner, c::IPPROTO_IP, c::IP_MULTICAST_LOOP)?;
//                                    Ok(raw != 0)
//                            }
//
//                            pub fn set_multicast_ttl_v4(&self, multicast_ttl_v4: u32) -> io::Result<()> {
//                                    setsockopt(&self.inner, c::IPPROTO_IP, c::IP_MULTICAST_TTL, multicast_ttl_v4 as c_int)
//                            }
//
//                            pub fn multicast_ttl_v4(&self) -> io::Result<u32> {
//                                    let raw: c_int = getsockopt(&self.inner, c::IPPROTO_IP, c::IP_MULTICAST_TTL)?;
//                                    Ok(raw as u32)
//                            }
//
//                            pub fn set_multicast_loop_v6(&self, multicast_loop_v6: bool) -> io::Result<()> {
//                                    setsockopt(&self.inner, c::IPPROTO_IPV6, c::IPV6_MULTICAST_LOOP, multicast_loop_v6 as c_int)
//                            }
//
//                            pub fn multicast_loop_v6(&self) -> io::Result<bool> {
//                                    let raw: c_int = getsockopt(&self.inner, c::IPPROTO_IPV6, c::IPV6_MULTICAST_LOOP)?;
//                                    Ok(raw != 0)
//                            }
//
//                            pub fn join_multicast_v4(&self, multiaddr: &Ipv4Addr, interface: &Ipv4Addr)
//                            -> io::Result<()> {
//                                    let mreq = c::ip_mreq {
//                                        imr_multiaddr: *multiaddr.as_inner(),
//                                            imr_interface: *interface.as_inner(),
//                                    };
//                                    setsockopt(&self.inner, c::IPPROTO_IP, c::IP_ADD_MEMBERSHIP, mreq)
//                            }
//
//                            pub fn join_multicast_v6(&self, multiaddr: &Ipv6Addr, interface: u32)
//                            -> io::Result<()> {
//                                    let mreq = c::ipv6_mreq {
//                                        ipv6mr_multiaddr: *multiaddr.as_inner(),
//                                            ipv6mr_interface: to_ipv6mr_interface(interface),
//                                    };
//                                    setsockopt(&self.inner, c::IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, mreq)
//                            }
//
//                            pub fn leave_multicast_v4(&self, multiaddr: &Ipv4Addr, interface: &Ipv4Addr)
//                            -> io::Result<()> {
//                                    let mreq = c::ip_mreq {
//                                        imr_multiaddr: *multiaddr.as_inner(),
//                                            imr_interface: *interface.as_inner(),
//                                    };
//                                    setsockopt(&self.inner, c::IPPROTO_IP, c::IP_DROP_MEMBERSHIP, mreq)
//                            }
//
//                            pub fn leave_multicast_v6(&self, multiaddr: &Ipv6Addr, interface: u32)
//                            -> io::Result<()> {
//                                    let mreq = c::ipv6_mreq {
//                                        ipv6mr_multiaddr: *multiaddr.as_inner(),
//                                            ipv6mr_interface: to_ipv6mr_interface(interface),
//                                    };
//                                    setsockopt(&self.inner, c::IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP, mreq)
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
//                            pub fn take_error(&self) -> io::Result<Option<io::Error>> {
//                                    self.inner.take_error()
//                            }
//
//                            pub fn set_nonblocking(&self, nonblocking: bool) -> io::Result<()> {
//                                    self.inner.set_nonblocking(nonblocking)
//                            }
//
//                            pub fn recv(&self, buf: &mut [u8]) -> io::Result<usize> {
//                                    self.inner.read(buf)
//                            }
//
//                            pub fn peek(&self, buf: &mut [u8]) -> io::Result<usize> {
//                                    self.inner.peek(buf)
//                            }
//
//                            pub fn send(&self, buf: &[u8]) -> io::Result<usize> {
//                                    let len = cmp::min(buf.len(), <wrlen_t>::max_value() as usize) as wrlen_t;
//                                    let ret = cvt(unsafe {
//                                        c::send(*self.inner.as_inner(),
//                                                buf.as_ptr() as *const c_void,
//                                                len,
//                                        MSG_NOSIGNAL)
//                                    })?;
//                                    Ok(ret as usize)
//                            }
//
//                            pub fn connect(&self, addr: io::Result<&SocketAddr>) -> io::Result<()> {
//                                    let (addrp, len) = addr?.into_inner();
//                                    cvt_r(|| unsafe { c::connect(*self.inner.as_inner(), addrp, len) }).map(|_| ())
//                            }
//                        }
//
//                        impl FromInner<Socket> for UdpSocket {
//                            fn from_inner(socket: Socket) -> UdpSocket {
//                                    UdpSocket { inner: socket }
//                            }
//                        }
//
//                        impl fmt::Debug for UdpSocket {
//                            fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
//                            let mut res = f.debug_struct("UdpSocket");
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
        constexpr bool as_raw_socket<net::UdpSocket> = true;

        template<>
        constexpr bool from_raw_socket<net::UdpSocket> = true;

        template<>
        constexpr bool into_raw_socket<net::UdpSocket> = true;

    } // namespace trait

} // namespace laio

#endif // UDPSOCKET_H
