#pragma once

#include "glfw_handle.h"
#include <glad/glfw.h>

namespace fractal
{

struct MainWindow {
    MainWindow();

    void loop();

private:
    GlfwHandle glfw_handle_;
    std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> window_;
};

}  // namespace fractal
