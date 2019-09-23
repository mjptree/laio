#ifndef OVERLOAD_H
#define OVERLOAD_H

template<class... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
Overload(Ts...) -> Overload<Ts...>;

#endif // OVERLOAD_H
