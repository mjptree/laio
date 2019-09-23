cmake_minimum_required(VERSION 3.1)

# Collect all header files
set(laio_iocp_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/CompletionPort.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CompletionStatus.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Handle.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Overlapped.h
        )

# Define target
add_library(laio_iocp
        INTERFACE
        )
target_sources(laio_iocp
        INTERFACE
            "$<BUILD_INTERFACE:${laio_iocp_headers}>"
        )
target_include_directories(laio_iocp
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/
            utils
        )