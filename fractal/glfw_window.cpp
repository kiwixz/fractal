#include "glfw_window.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace fractal {

GlfwWindow::GlfwWindow(int width, int height, std::string_view title,
                       GLFWmonitor* monitor, GLFWwindow* share) :
    window_{nullptr, glfwDestroyWindow}
{
    GLFWwindow* window = glfwCreateWindow(width, height, title.data(), monitor, share);
    if (!window)
        throw std::runtime_error{"could not create window"};
    window_.reset(window);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader([](char const* name) {
            return reinterpret_cast<void*>(glfwGetProcAddress(name));
        }))
        throw std::runtime_error{"could not load opengl"};

    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei /*length*/, GLchar const* message, void const* /*userParam*/) {
        spdlog::debug("[opengl] source:{} type:{} id:{} severity:{}\n\t {}",
                      source, type, id, severity, message);
    },
                           nullptr);

    spdlog::info("opened window,\n"
                 "\t opengl version: {}\n"
                 "\t glsl version: {}",
                 glGetString(GL_VERSION),
                 glGetString(GL_SHADING_LANGUAGE_VERSION));
}

GlfwWindow::operator GLFWwindow*()
{
    return window_.get();
}

GlfwWindow::operator GLFWwindow const*() const
{
    return window_.get();
}

GLFWwindow* GlfwWindow::ptr()
{
    return window_.get();
}

GLFWwindow const* GlfwWindow::ptr() const
{
    return window_.get();
}

}  // namespace fractal
