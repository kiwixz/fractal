#pragma once

#include "gl_object.h"
#include "scope_exit.h"

namespace fractal {

struct FullQuad {
    FullQuad();

    [[nodiscard]] ScopeExit<> bind();
    void draw() const;  ///< Needs to be bound first.

private:
    GlBuffers<> vertex_buffer_;
    GlVertexArrays<> vertex_array_;

    static void unbind();
};

}  // namespace fractal
