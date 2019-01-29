#pragma once

#include "glfw_handle.h"
#include <glad/glfw.h>
#include <memory>
#include <string_view>

namespace fractal {

struct GlfwWindow {
    GlfwWindow(int width, int height, std::string_view title,
               GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

    operator GLFWwindow*();
    operator GLFWwindow const*() const;
    GLFWwindow* ptr();
    GLFWwindow const* ptr() const;

private:
    GlfwHandle glfw_handle_;
    std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> window_;
};

}  // namespace fractal
