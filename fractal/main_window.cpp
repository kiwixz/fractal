#include "main_window.h"
#include "full_quad.h"
#include "glad_glfw.h"
#include "glfw_handle.h"
#include "glfw_window.h"
#include "mandelbrot.h"
#include "settings.h"
#include "shader.h"
#include "texture.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace fractal {
namespace {

struct MainWindow {
    explicit MainWindow(Settings const& settings);

    void loop();

private:
    struct Glfw {
        Glfw();

    private:
        GlfwHandle handle_;
    };

    Glfw glfw_;
    GlfwWindow window_;
    ShaderProgram program_;
    FullQuad quad_;
    Texture texture_;
    Mandelbrot mandelbrot_;

    double x_;
    double y_;
    double zoom_;
    double zoom_speed_;

    void on_key(int key, int scancode, int action, int mods);
    void on_resize(int width, int height);
};


MainWindow::Glfw::Glfw()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
}


MainWindow::MainWindow(Settings const& settings) :
    window_{settings.width, settings.height, "fractal"},
    texture_{settings.width, settings.height},
    mandelbrot_{settings.width, settings.height},
    x_{settings.x},
    y_{settings.y},
    zoom_{settings.zoom},
    zoom_speed_{settings.zoom_speed}
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

    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point last_frame = Clock::now();

    while (!glfwWindowShouldClose(window_)) {
        glClear(GL_COLOR_BUFFER_BIT);

        uint32_t const* pixels = mandelbrot_.generate(x_, y_, zoom_);
        texture_.update(pixels, GL_BGRA);
        quad_.draw();

        glfwSwapBuffers(window_);
        glfwPollEvents();

        Clock::time_point now = Clock::now();
        double delta = std::chrono::duration<double>(now - last_frame).count();
        last_frame = now;
        spdlog::info("[main_window] frame time: {}ms", delta * std::milli::den);

        zoom_ += zoom_ * zoom_speed_ * delta;

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

}  // namespace


void show_main_window(Settings const& settings)
{
    MainWindow{settings}.loop();
}

}  // namespace fractal
