#pragma once

#include "gl_object.h"
#include <array>

namespace fractal {

class Quad {
public:
    Quad();

private:
    GlVertexArrays<> vertex_array_;
    GlBuffers<2> vertex_buffers_;
};

}  // namespace fractal
