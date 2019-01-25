#pragma once

#include "glad/glfw.h"
#include "glfw_handle.h"

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
