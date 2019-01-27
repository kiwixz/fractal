#pragma once

#include "gl_object.h"
#include "scope_exit.h"
#include <glad/glad.h>

namespace fractal {

struct TextureStream {
    TextureStream(int width, int height, GLenum format = GL_RGB8);

    [[nodiscard]] ScopeExit bind();

    template <typename T>
    void update(T const* pixels,
                GLenum format = GL_BGR, GLenum type = GL_UNSIGNED_BYTE,
                int x_offset = 0, int y_offset = 0, int width = 0, int height = 0);

private:
    GlTextures<> texture_;
    int width_;
    int height_;
};


template <typename T>
void TextureStream::update(T const* pixels, GLenum format, GLenum type, int x_offset, int y_offset, int width, int height)
{
    glTextureSubImage2D(texture_, 0,
                        x_offset, y_offset,
                        width != 0 ? width : width_,
                        height != 0 ? height : height_,
                        format, type, pixels);
}

}  // namespace fractal
