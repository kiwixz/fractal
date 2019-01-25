#include "gl_object.h"

namespace fractal {

template <GlDeleter Tdeleter>
GlObject<Tdeleter>::GlObject() :
    handle_{nullptr, [this](auto) {
                deleter(id_);
            }}
{}

template <GlDeleter Tdeleter>
GlObject<Tdeleter>::operator GLuint() const
{
    return id_;
}

template <GlDeleter Tdeleter>
GLuint GlObject<Tdeleter>::id() const
{
    return id_;
}


template <GlArrayDeleter Tdeleter, size_t Tsize>
GlArrayObject<Tdeleter, Tsize>::GlArrayObject() :
    handle_{nullptr, [this](auto) {
                deleter(size, ids_.data());
            }}
{}

template <GlArrayDeleter Tdeleter, size_t Tsize>
GLuint GlArrayObject<Tdeleter, Tsize>::operator[](size_t index) const
{
    return ids_[index];
}

}  // namespace fractal
