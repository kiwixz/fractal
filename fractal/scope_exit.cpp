#include "scope_exit.h"

namespace fractal {

ScopeExit::ScopeExit(std::function<void()> function) :
    function_{std::move(function)}
{}

ScopeExit::~ScopeExit()
{
    if (function_)
        function_();
}

ScopeExit::ScopeExit(ScopeExit&& other)
{
    *this = std::move(other);
}

ScopeExit& ScopeExit::operator=(ScopeExit&& other)
{
    std::swap(function_, other.function_);
    return *this;
}

}  // namespace fractal
