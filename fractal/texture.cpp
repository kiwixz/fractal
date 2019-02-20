#include "texture.h"

namespace fractal {

Texture::Texture(int width, int height, GLenum format) :
    width_{width}, height_{height}
{
    glCreateTextures(GL_TEXTURE_2D, 1, texture_.ptr());
    glTextureStorage2D(texture_[0], 1, format, width, height);
}

ScopeExit Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, texture_[0]);
    return ScopeExit{[] {
        glBindTexture(GL_TEXTURE_2D, 0);
    }};
}

}  // namespace fractal
