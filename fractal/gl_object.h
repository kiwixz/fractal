#pragma once

#include <glad/glad.h>
#include <array>
#include <functional>
#include <memory>

namespace fractal {

using GlDeleter = void (*)(GLuint id);
using GlArrayDeleter = void (*)(GLsizei size, GLuint id);


template <GlDeleter Tdeleter>
struct GlObject {
    static constexpr GlDeleter deleter = Tdeleter;

    GlObject();

    operator GLuint() const;
    GLuint id() const;

private:
    std::unique_ptr<void, std::function<void(void*)>> handle_;
    GLuint id_;
};


template <GlArrayDeleter Tdeleter, size_t Tsize = 1>
struct GlArrayObject {
    static constexpr GlDeleter deleter = Tdeleter;
    static constexpr size_t size = Tsize;

    GlArrayObject();

    GLuint operator[](size_t index) const;

private:
    std::unique_ptr<void, std::function<void(void*)>> handle_;
    std::array<GLuint, size> ids_;
};

}  // namespace fractal

#include "gl_object.inl"
