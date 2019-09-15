cmake_minimum_required(VERSION 3.1)

add_library(net
        SocketAddrBuf.cpp
        AcceptAddrBuf.cpp
        AcceptAddr.cpp
        WsaExtension.cpp
        SocketAddr.cpp
        TcpListener.cpp
        TcpStream.cpp
        UdpSocket.cpp
        )
target_include_directories(net
        PUBLIC
            ./
            ../utils
            interfaces
        )