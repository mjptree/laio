# Arbitrary - We probably do not need any fancy functionality
cmake_minimum_required(VERSION 3.1)
project(laio)

set(CMAKE_CXX_STANDARD 17)

# Subdirectories
if(NOT TARGET windows_system_error)
    add_subdirectory(dependencies/windows_system_error)
endif()

# Use GSL for acces to spans
# TODO: To be replaced by STL span upon availability
if(NOT TARGET GSL)
    add_subdirectory(dependencies/GSL)
endif()

if(NOT TARGET Catch2)
    add_subdirectory(dependencies/Catch2)
endif()

# Build library
add_library(laio
        src/Handle.cpp
        src/CompletionPort.cpp
        src/CompletionStatus.cpp
        src/Overlapped.cpp
        )
target_include_directories(laio
        PUBLIC
            include
        #PRIVATE
            src/utils
        )
add_subdirectory(src/net)
target_link_libraries(laio windows_system_error GSL net)

# Build tests
add_executable(test_laio
        test/testmain.cpp
        test/test_laio.cpp
        )
target_link_libraries(test_laio windows_system_error Catch2 laio)