#pragma once

#include "gl_object.h"
#include "scope_exit.h"
#include <glad/glad.h>

namespace fractal {

struct ShaderProgram;


struct Shader {
    Shader(GLenum type, std::string_view source);

private:
    GlShader shader_;

    friend ShaderProgram;
};


struct ShaderProgram {
public:
    ShaderProgram();

    void attach(Shader const& shader);
    void link();

    [[nodiscard]] ScopeExit<> bind();

private:
    GlProgram program_;

    static void unbind();
};

}  // namespace fractal
