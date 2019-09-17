cmake_minimum_required(VERSION 3.1)

# Subdirectories
if(NOT TARGET windows_system_error)
    add_subdirectory(../../dependencies/windows_system_error)
endif()

# Use GSL for acces to spans
# TODO: To be replaced by STL span upon availability
if(NOT TARGET GSL)
    add_subdirectory(../../dependencies/GSL)
endif()

add_library(net
        SocketAddrBuf.cpp
        AcceptAddrBuf.cpp
        AcceptAddr.cpp
        WsaExtension.cpp
        SocketAddr.cpp
        TcpListener.cpp
        TcpStream.cpp
        UdpSocket.cpp
        Socket.cpp
        )
target_include_directories(net
        PUBLIC
            ./
            ../utils
            types
            interfaces
            ../../include
        )
target_link_libraries(net windows_system_error GSL)