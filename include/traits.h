#ifndef TRAITS_H
#define TRAITS_H

namespace laio {

    template<typename T>
    constexpr bool send = false;

    template<typename T>
    constexpr bool sync = false;

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

} // namespace laio

#endif // TRAITS_H
