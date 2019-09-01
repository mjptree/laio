#ifndef OVERLOAD_H
#define OVERLOAD_H

template<class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
overload(Ts...) -> overload<Ts...>;

#endif // OVERLOAD_H
