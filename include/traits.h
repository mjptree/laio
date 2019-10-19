#pragma once

namespace laio::trait {

    template<typename T>
    constexpr bool is_send = false;

    template<typename T>
    constexpr bool is_sync = false;

    template<typename T>
    constexpr bool as_raw_handle = false;

    template<typename T>
    constexpr bool from_raw_handle = false;

    template<typename T>
    constexpr bool into_raw_handle = false;

    template<typename T>
    constexpr bool as_raw_socket = false;

    template<typename T>
    constexpr bool from_raw_socket = false;

    template<typename T>
    constexpr bool into_raw_socket = false;

} // namespace laio::trait
