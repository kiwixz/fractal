#include "main_window.h"
#include "full_quad.h"
#include "shader.h"
#include "texture_stream.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace fractal {
namespace {

constexpr int base_width = 1280;
constexpr int base_height = 720;

}  // namespace


MainWindow::MainWindow() :
    window_{nullptr, glfwDestroyWindow}
{
    glfwSetErrorCallback([](int error, char const* description) {
        spdlog::error("[glfw] error {}: {}", error, description);
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    GLFWwindow* window = glfwCreateWindow(base_width, base_height, "fractal", nullptr, nullptr);
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

    glfwSetWindowUserPointer(window, this);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        return reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(win))->on_key(key, scancode, action, mods);
    });
}

void MainWindow::loop()
{
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

    ShaderProgram program;
    program.attach({GL_VERTEX_SHADER, vertex_source});
    program.attach({GL_FRAGMENT_SHADER, fragment_source});
    program.link();
    ScopeExit shader_binding = program.bind();

    FullQuad quad;
    ScopeExit quad_binding = quad.bind();

    TextureStream stream{base_width, base_height};
    ScopeExit stream_binding = stream.bind();

    mandelbrot_.resize(base_width, base_height);

    while (!glfwWindowShouldClose(window_.get())) {
        glClear(GL_COLOR_BUFFER_BIT);
        stream.update(mandelbrot_.generate());
        quad.draw();
        glfwSwapBuffers(window_.get());
        glfwPollEvents();
    }
}

void MainWindow::on_key(int key, int /*scancode*/, int action, int mods)
{
    if (action == GLFW_RELEASE) {
        if (mods == 0) {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window_.get(), true);
        }
    }
}

}  // namespace fractal
