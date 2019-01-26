#pragma once

namespace fractal {

template <typename T, typename... Args>
auto bind_front(T&& function, Args&&... args);


template <typename Function, typename... Args>
auto bind_front(Function&& function, Args&&... args)
{
    return [function = std::forward<Function>(function),
            bound_args = std::make_tuple(std::forward<Args>(args)...)](auto&& call_args...) {
        std::apply([&](auto&&... bound_args) {
            return std::invoke(function,
                               std::forward<Args>(bound_args)...,
                               std::forward<decltype(call_args)>(call_args)...);
        },
                   decltype(bound_args){bound_args});
    };
}

}  // namespace fractal
