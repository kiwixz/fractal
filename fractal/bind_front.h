#pragma once

namespace fractal {

template <typename T, typename... Args>
auto bind_front(T&& function, Args&&... args);

}  // namespace fractal

#include "bind_front.inl"
