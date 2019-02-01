#include "main_window.h"
#include "settings.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace fractal {

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
    window_{settings::get().width, settings::get().height, "fractal"},
    texture_{settings::get().width, settings::get().height},
    mandelbrot_{settings::get().width, settings::get().height}
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
    ScopeExit texture_binding = texture_.bind();

    while (!glfwWindowShouldClose(window_)) {
        glClear(GL_COLOR_BUFFER_BIT);

        uint32_t const* pixels = mandelbrot_.generate(settings::get().x,
                                                      settings::get().y,
                                                      settings::get().zoom);
        texture_.update(pixels, GL_BGRA);
        quad_.draw();

        glfwSwapBuffers(window_);
        glfwPollEvents();

        texture_binding.cancel();
        texture_binding = texture_.bind();  // texture may have been resized
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
    texture_ = {width, height};
    mandelbrot_.resize(width, height);
}

}  // namespace fractal
