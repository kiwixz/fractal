#include "mandelbrot.h"

namespace fractal {

Mandelbrot::Mandelbrot(int width, int height)
{
    resize(width, height);
}

void Mandelbrot::resize(int width, int height)
{
    width_ = width;
    height_ = height;
    pixels_.resize(width * height * 3);
}

uint8_t* Mandelbrot::generate()
{
    for (uint8_t& color : pixels_)
        color = rand();

    return pixels_.data();
}

}  // namespace fractal
