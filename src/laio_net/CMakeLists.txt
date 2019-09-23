cmake_minimum_required(VERSION 3.1)

# Collect all header files
set(laio_net_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/AcceptAddr.h
        ${CMAKE_CURRENT_SOURCE_DIR}/AcceptAddrBuf.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Socket.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SocketAddr.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SocketAddrBuf.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SocketAddrV4.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SocketAddrV6.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TcpListener.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TcpStream.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UdpSocket.h
        ${CMAKE_CURRENT_SOURCE_DIR}/WsaExtension.h
        )

# Collect interfaces
set(laio_net_interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/TcpListenerExt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/TcpStreamExt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/UdpSocketExt.h
        )

# Define target
add_library(net
        INTERFACE
        )
target_sources(net
        INTERFACE
            "$<BUILD_INTERFACE:${laio_net_headers};${laio_net_interfaces}>"
        )
target_include_directories(net
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/
            utils
            interfaces
        )