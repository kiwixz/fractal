#pragma once

#include "gl_object.h"
#include "scope_exit.h"
#include <array>

namespace fractal {

class Quad {
public:
    Quad();

    [[nodiscard]] ScopeExit bind();
    void draw() const;  ///< Needs to be bound.

private:
    GlBuffers<2> vertex_buffers_;
    GlVertexArrays<> vertex_array_;
};

}  // namespace fractal
