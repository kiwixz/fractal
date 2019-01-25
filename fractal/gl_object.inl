#include "gl_object.h"

namespace fractal
{

template <typename T>
GlObject::GlObject(GLuint id, T&& deleter) :
    handle_{nullptr, std::forward<T>(deleter)},
    id_{id}
{}

}  // namespace fractal
