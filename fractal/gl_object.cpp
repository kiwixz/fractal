#include "gl_object.h"

namespace fractal
{

GlObject::GlObject() :
    handle_{nullptr, nullptr}
{}

GlObject::operator GLuint() const
{
    return id_;
}

GLuint GlObject::id() const
{
    return id_;
}

}  // namespace fractal
