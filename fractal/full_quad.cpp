#include "full_quad.h"
#include <glad/glad.h>

namespace fractal {

FullQuad::FullQuad()
{
    constexpr std::array position{-1.f, -1.f,
                                  1.f, -1.f,
                                  -1.f, 1.f,
                                  1.f, 1.f};

    glCreateVertexArrays(1, vertex_array_.ptr());
    glCreateBuffers(1, vertex_buffer_.ptr());

    glVertexArrayVertexBuffer(vertex_array_, 0, vertex_buffer_, 0, 2 * sizeof(GLfloat));
    glEnableVertexArrayAttrib(vertex_array_, 0);

    glVertexArrayAttribFormat(vertex_array_, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vertex_array_, 0, 0);
    glNamedBufferData(vertex_buffer_, sizeof(position), position.data(), GL_STATIC_DRAW);
}

ScopeExit FullQuad::bind()
{
    glBindVertexArray(vertex_array_);
    return {[] {
        glBindVertexArray(0);
    }};
}

void FullQuad::draw() const
{
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

}  // namespace fractal
