#pragma once

#include <functional>

namespace fractal {

struct ScopeExit {
    ScopeExit() = default;
    ScopeExit(std::function<void()> function);
    ~ScopeExit();
    ScopeExit(ScopeExit const&) = delete;
    ScopeExit& operator=(ScopeExit const&) = delete;
    ScopeExit(ScopeExit&& other) noexcept;
    ScopeExit& operator=(ScopeExit&& other) noexcept;

private:
    std::function<void()> function_;
};

}  // namespace fractal
