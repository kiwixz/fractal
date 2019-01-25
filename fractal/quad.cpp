#include "quad.h"

namespace fractal {

Quad::Quad()
{
    constexpr std::array<GLfloat, 4 * 2> texture_coords{{0.f, 0.f,
                                                         1.f, 0.f,
                                                         0.f, 1.f,
                                                         1.f, 1.f}};

    glGenBuffers(vertex_buffers_.size(), vertex_buffers_.ptr());

    glGenVertexArrays(1, vertex_array_.ptr());
    glBindVertexArray(vertex_array_);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers_[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers_[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Quad::draw() const
{
    glBindVertexArray(vertex_array_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

}  // namespace fractal
