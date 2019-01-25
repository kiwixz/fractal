#include "gl_object.h"

namespace fractal
{

GlObject::operator GLuint() const
{
    return id_;
}

GLuint GlObject::id() const
{
    return id_;
}

}  // namespace fractal
