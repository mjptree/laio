#pragma once

#include <TcpListenerExt.h>
#include <TcpStreamExt.h>
#include <UdpSocketExt.h>

namespace laio::interface {

    using TcpListenerExt = laio::net::interface::TcpListenerExt;
    using TcpStreamExt = laio::net::interface::TcpStreamExt;
    using UdpSocketExt = laio::net::interface::UdpSocketExt;

} // namespace laio::interface
