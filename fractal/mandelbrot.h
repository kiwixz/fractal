#pragma once

#include <cstdint>
#include <vector>

namespace fractal {

struct Mandelbrot {
    Mandelbrot() = default;
    Mandelbrot(int width, int height);

    void resize(int width, int height);
    uint32_t* generate();  ///< pointer may be invalidated after any interaction with this object

private:
    std::vector<uint32_t> pixels_;
    int width_;
    int height_;

    int max_iterations_ = 64;
    float x_offset_ = -2.5f;
    float y_offset_ = -1.f;
    float x_size_ = 1.f - x_offset_;
    float y_size_ = 1.f - y_offset_;

    uint32_t color(int iterations);
};

}  // namespace fractal
