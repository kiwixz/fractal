#pragma once

#include "full_quad.h"
#include "glfw_handle.h"
#include "glfw_window.h"
#include "mandelbrot.h"
#include "shader.h"
#include "texture_stream.h"
#include <glad/glfw.h>

namespace fractal {

struct MainWindow {
    MainWindow();

    void loop();

private:
    struct Glfw {
        Glfw();

    private:
        GlfwHandle handle_;
    };

    GlfwWindow window_;
    ShaderProgram program_;
    FullQuad quad_;
    TextureStream stream_;
    Mandelbrot mandelbrot_;

    void on_key(int key, int scancode, int action, int mods);
    void on_resize(int width, int height);
};

}  // namespace fractal
