#pragma once

#include <functional>

namespace fractal {

template <typename T, typename... Args>
auto bind_front(T&& callable, Args&&... args);


template <typename Function, typename... Args>
auto bind_front(Function&& callable, Args&&... args)
{
    return [callable = std::forward<Function>(callable),
            bound_args_tuple = std::make_tuple(std::forward<Args>(args)...)](auto&& call_args...) {
        std::apply([&](auto&&... bound_args) {
            return std::invoke(callable,
                               std::forward<Args>(bound_args)...,
                               std::forward<decltype(call_args)>(call_args)...);
        },
                   decltype(bound_args_tuple){bound_args_tuple});
    };
}

}  // namespace fractal
