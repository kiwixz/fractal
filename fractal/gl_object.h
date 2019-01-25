#pragma once

#include <glad/glad.h>
#include <memory>

namespace fractal
{

struct GlObject {
    GlObject();

    template <typename T>
    GlObject(GLuint id, T&& deleter);

    operator GLuint() const;
    GLuint id() const;

private:
    std::unique_ptr<void, void (*)(void*)> handle_;
    GLuint id_;
};

}  // namespace fractal

#include "gl_object.inl"
