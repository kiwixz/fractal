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


template <GlDeleter* Tdeleter>
GlObject<Tdeleter>::GlObject(GLuint id) :
    id_{id}
{}

template <GlDeleter* Tdeleter>
GlObject<Tdeleter>::~GlObject()
{
    if (id_ != 0)
        (*Tdeleter)(id_);
}

template <GlDeleter* Tdeleter>
GlObject<Tdeleter>::GlObject(GlObject&& other) noexcept
{
    *this = std::move(other);
}

template <GlDeleter* Tdeleter>
GlObject<Tdeleter>& GlObject<Tdeleter>::operator=(GlObject&& other) noexcept
{
    std::swap(id_, other.id_);
    return *this;
}

template <GlDeleter* Tdeleter>
GLuint GlObject<Tdeleter>::id() const
{
    return id_;
}

template <GlDeleter* Tdeleter>
GLuint* GlObject<Tdeleter>::ptr()
{
    return &id_;
}


template <GlArrayDeleter* Tdeleter, size_t Tsize>
GlArrayObject<Tdeleter, Tsize>::~GlArrayObject()
{
    if (ids_[0] != 0)
        (*Tdeleter)(static_cast<GLsizei>(ids_.size()), ids_.data());
}

template <GlArrayDeleter* Tdeleter, size_t Tsize>
GlArrayObject<Tdeleter, Tsize>::GlArrayObject(GlArrayObject&& other) noexcept
{
    *this = std::move(other);
}

template <GlArrayDeleter* Tdeleter, size_t Tsize>
GlArrayObject<Tdeleter, Tsize>& GlArrayObject<Tdeleter, Tsize>::operator=(GlArrayObject&& other) noexcept
{
    std::swap(ids_, other.ids_);
    return *this;
}

template <GlArrayDeleter* Tdeleter, size_t Tsize>
GLuint GlArrayObject<Tdeleter, Tsize>::operator[](size_t index) const
{
    return ids_[index];
}

template <GlArrayDeleter* Tdeleter, size_t Tsize>
constexpr GLsizei GlArrayObject<Tdeleter, Tsize>::size() const
{
    return static_cast<GLsizei>(ids_.size());
}

template <GlArrayDeleter* Tdeleter, size_t Tsize>
GLuint* GlArrayObject<Tdeleter, Tsize>::ptr()
{
    return ids_.data();
}

}  // namespace fractal
