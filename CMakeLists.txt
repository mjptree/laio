# Arbitrary - We probably do not need any fancy functionality
cmake_minimum_required(VERSION 3.1)
project(laio)

set(CMAKE_CXX_STANDARD 17)

# Subdirectories
if(NOT TARGET windows_system_error)
    add_subdirectory(dependencies/windows_system_error)
endif()

# Use GSL for access to spans
# TODO: Replace by STL span upon availability
if(NOT TARGET GSL)
    add_subdirectory(dependencies/GSL)
endif()

# Use fmt for access to format
# TODO: Replace by STL format upon availability
if(NOT TARGET fmt)
    add_subdirectory(dependencies/fmt)
endif()

if(NOT TARGET Catch2)
    add_subdirectory(dependencies/Catch2)
endif()

# Collect public interfaces
set(laio_public_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/include/interfaces.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/traits.h
        )

# Build library
add_library(laio
        INTERFACE
        )
target_sources(laio
        INTERFACE
            "$<BUILD_INTERFACE:${laio_public_headers}>"
        )
target_include_directories(laio
        INTERFACE
            include
        )
add_subdirectory(src/laio_iocp)
add_subdirectory(src/laio_net)
target_link_libraries(laio
        INTERFACE
            windows_system_error
            GSL
            fmt
            laio_iocp
            laio_net
        )

# Build tests
add_executable(test_laio
        test/testmain.cpp
        test/test_laio_iocp.cpp
        test/test_laio_net.cpp
        )
target_link_libraries(test_laio windows_system_error Catch2 laio)