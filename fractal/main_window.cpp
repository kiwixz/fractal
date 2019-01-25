#include "main_window.h"
#include <stdexcept>

namespace fractal {

MainWindow::MainWindow() :
    window_{nullptr, glfwDestroyWindow}
{
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
    while (!glfwWindowShouldClose(window_.get())) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(0, 0);

        glfwSwapBuffers(window_.get());
        glfwPollEvents();
    }
}

}  // namespace fractal
