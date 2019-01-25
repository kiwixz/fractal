#include "main_window.h"
#include "full_quad.h"
#include "shader.h"
#include <stdexcept>

namespace fractal {

MainWindow::MainWindow() :
    window_{nullptr, glfwDestroyWindow}
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "fractal", nullptr, nullptr);
    if (!window)
        throw std::runtime_error{"could not create window"};
    window_.reset(window);

    glfwMakeContextCurrent(window);
    if (!gladLoadGL())
        throw std::runtime_error{"could not load opengl"};

    glfwSwapInterval(1);
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
                color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
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

}  // namespace fractal
