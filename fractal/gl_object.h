#pragma once

#include <glad/glad.h>
#include <array>

namespace fractal {

using GlDeleter = void (*)(GLuint id);
using GlArrayDeleter = void (*)(GLsizei size, GLuint const* ids);


// we need to take pointers to make deleters compile time constants
// because they are loaded at runtime
template <GlDeleter* Tdeleter>
struct GlObject {
    GlObject() = default;
    explicit GlObject(GLuint id);
    ~GlObject();
    GlObject(GlObject const&) = delete;
    GlObject& operator=(GlObject const&) = delete;
    GlObject(GlObject&& other) noexcept;
    GlObject& operator=(GlObject&& other) noexcept;

    operator GLuint() const;
    GLuint id() const;

    GLuint* ptr();

private:
    GLuint id_ = 0;
};


template <GlArrayDeleter* Tdeleter, size_t Tsize = 1>
struct GlArrayObject {
    GlArrayObject() = default;
    ~GlArrayObject();
    GlArrayObject(GlArrayObject const&) = delete;
    GlArrayObject& operator=(GlArrayObject const&) = delete;
    GlArrayObject(GlArrayObject&& other) noexcept;
    GlArrayObject& operator=(GlArrayObject&& other) noexcept;

    operator GLuint() const;
    GLuint operator[](size_t index) const;
    constexpr GLsizei size() const;

    GLuint* ptr();

private:
    std::array<GLuint, Tsize> ids_ = {};
};


template <size_t Tsize = 1>
using GlBuffers = GlArrayObject<&glDeleteBuffers, Tsize>;
template <size_t Tsize = 1>
using GlFrameBuffers = GlArrayObject<&glDeleteFramebuffers, Tsize>;
template <size_t Tsize = 1>
using GlQueries = GlArrayObject<&glDeleteQueries, Tsize>;
template <size_t Tsize = 1>
using GlRenderBuffers = GlArrayObject<&glDeleteRenderbuffers, Tsize>;
template <size_t Tsize = 1>
using GlSamplers = GlArrayObject<&glDeleteSamplers, Tsize>;
template <size_t Tsize = 1>
using GlTextures = GlArrayObject<&glDeleteTextures, Tsize>;
template <size_t Tsize = 1>
using GlVertexArrays = GlArrayObject<&glDeleteVertexArrays, Tsize>;

using GlProgram = GlObject<&glDeleteProgram>;
using GlShader = GlObject<&glDeleteShader>;

}  // namespace fractal

#include "gl_object.inl"
