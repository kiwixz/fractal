#pragma once

#include <vector>

namespace fractal {

struct Mandelbrot {
    Mandelbrot() = default;
    Mandelbrot(int width, int height);

    void resize(int width, int height);
    uint8_t* generate();  ///< pointer may be invalidated after any interaction with this object

private:
    std::vector<uint8_t> pixels_;
    int width_;
    int height_;
};

}  // namespace fractal
