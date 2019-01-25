#pragma once

#include "gl_object.h"
#include <array>

namespace fractal {

class Quad {
public:
    Quad();

    void draw() const;

private:
    GlBuffers<2> vertex_buffers_;
    GlVertexArrays<> vertex_array_;
};

}  // namespace fractal
