#pragma once

#include "gl_object.h"

namespace fractal {

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
GlObject<Tdeleter>::operator GLuint() const
{
    return id_;
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
GlArrayObject<Tdeleter, Tsize>::operator GLuint() const
{
    return ids_[0];
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
