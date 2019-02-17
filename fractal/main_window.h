#pragma once

#include "full_quad.h"
#include "glfw_handle.h"
#include "glfw_window.h"
#include "mandelbrot.h"
#include "settings.h"
#include "shader.h"
#include "texture.h"

namespace fractal {

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

}  // namespace fractal
