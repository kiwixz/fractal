#pragma once

#include "glfw_handle.h"
#include "mandelbrot.h"
#include <glad/glfw.h>

namespace fractal {

struct MainWindow {
    MainWindow();

    void loop();

private:
    GlfwHandle glfw_handle_;
    std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> window_;
    Mandelbrot mandelbrot_;

    void on_key(int key, int scancode, int action, int mods);
    void on_resize(int width, int height);
};

}  // namespace fractal
