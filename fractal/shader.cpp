#include "shader.h"
#include <fmt/format.h>
#include <spdlog/spdlog.h>

namespace fractal {

Shader::Shader(GLenum type, std::string_view source)
{
    GLuint shader = glCreateShader(type);
    GLchar const* source_ptr = source.data();
    GLint source_length = static_cast<GLint>(source.length());
    glShaderSource(shader, 1, &source_ptr, &source_length);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> info(length);
        glGetShaderInfoLog(shader, length, &length, info.data());
        throw std::runtime_error{fmt::format("could not compile shader: {}", info.data())};
    }

    spdlog::info("shader compiled");
}


ShaderProgram::ShaderProgram()
{
    program_ = glCreateProgram();
}

void ShaderProgram::attach(Shader const& shader)
{
    glAttachShader(program_, shader.shader_);
}

void ShaderProgram::link()
{
    glLinkProgram(program_);
    spdlog::info("shader program linked");
}

ScopeExit ShaderProgram::bind()
{
    glUseProgram(program_);
    return {[] {
        glUseProgram(0);
    }};
}

}  // namespace fractal
