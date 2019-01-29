#include "main_window.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace fractal {
namespace {

constexpr int base_width = 1280;
constexpr int base_height = 720;

}  // namespace


MainWindow::Glfw::Glfw()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
}


MainWindow::MainWindow() :
    window_{base_width, base_height, "fractal"},
    stream_{base_width, base_height},
    mandelbrot_{base_width, base_height, 128}
{
    glfwSetWindowUserPointer(window_, this);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        return reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window))->on_key(key, scancode, action, mods);
    });
    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
        return reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window))->on_resize(width, height);
    });

    constexpr std::string_view vertex_source = R"(
            #version 450
            layout(location=0) in vec2 pos;
            out vec2 tex_coords;
            void main() {
                gl_Position = vec4(pos, 0.0f, 1.0f);
                tex_coords = (pos + 1) / 2;
            }
        )";
    constexpr std::string_view fragment_source = R"(
            #version 450
            in vec2 tex_coords;
            out vec4 color;
            uniform sampler2D tex;
            void main() {
                color = texture2D(tex, tex_coords);
            }
        )";
    program_.attach({GL_VERTEX_SHADER, vertex_source});
    program_.attach({GL_FRAGMENT_SHADER, fragment_source});
    program_.link();
}

void MainWindow::loop()
{
    ScopeExit shader_binding = program_.bind();
    ScopeExit quad_binding = quad_.bind();
    ScopeExit stream_binding = stream_.bind();

    while (!glfwWindowShouldClose(window_)) {
        glClear(GL_COLOR_BUFFER_BIT);
        stream_.update(mandelbrot_.generate(), GL_BGRA);
        quad_.draw();

        glfwSwapBuffers(window_);
        glfwPollEvents();

        stream_binding.cancel();
        stream_binding = stream_.bind();  // stream may have been resized
    }
}

void MainWindow::on_key(int key, int /*scancode*/, int action, int mods)
{
    if (action == GLFW_RELEASE) {
        if (mods == 0) {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window_, true);
        }
    }
}

void MainWindow::on_resize(int width, int height)
{
    glViewport(0, 0, width, height);
    stream_ = {width, height};
    mandelbrot_.resize(width, height);
}

}  // namespace fractal
