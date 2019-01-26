#include "main_window.h"
#include "full_quad.h"
#include "shader.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace fractal {

MainWindow::MainWindow() :
    window_{nullptr, glfwDestroyWindow}
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    GLFWwindow* window = glfwCreateWindow(1600, 900, "fractal", nullptr, nullptr);
    if (!window)
        throw std::runtime_error{"could not create window"};
    window_.reset(window);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader([](const char* name) {
            return reinterpret_cast<void*>(glfwGetProcAddress(name));
        }))
        throw std::runtime_error{"could not load opengl"};

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
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        return reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(win))->on_framebuffer_size(width, height);
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
            void main() {
                color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
            }
        )";

    ShaderProgram program;
    program.attach(Shader{GL_VERTEX_SHADER, vertex_source});
    program.attach(Shader{GL_FRAGMENT_SHADER, fragment_source});
    program.link();
    ScopeExit shader_binding = program.bind();

    FullQuad quad;
    ScopeExit quad_binding = quad.bind();
    while (!glfwWindowShouldClose(window_.get())) {
        glClear(GL_COLOR_BUFFER_BIT);
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

void MainWindow::on_framebuffer_size(int width, int height)
{
    glViewport(0, 0, width, height);
}

}  // namespace fractal
