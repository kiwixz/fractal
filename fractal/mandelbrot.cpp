#include "mandelbrot.h"
#include <spdlog/spdlog.h>
#include <chrono>

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
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start = Clock::now();

    float dx = x_size_ / width_;
    float dy = y_size_ / height_;

    for (int pixel_y = 0; pixel_y < height_; ++pixel_y)
        for (int pixel_x = 0; pixel_x < width_; ++pixel_x) {
            float real_x = x_offset_ + dx * pixel_x;
            float real_y = y_offset_ + dy * pixel_y;
            float x = 0;
            float y = 0;

            int iterations = 0;
            while (iterations < max_iterations_ && x * x + y * y <= 2 * 2) {
                if (x * x + y * y > 2 * 2)
                    break;

                float x_tmp = x * x - y * y + real_x;
                float y_tmp = 2 * x * y + real_y;
                if (x_tmp == x && y_tmp == y)
                    break;

                x = x_tmp;
                y = y_tmp;
                ++iterations;
            }
            color(pixel_x, pixel_y, iterations);
        }

    Clock::time_point end = Clock::now();
    spdlog::info("[mandelbrot] generated in {}ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    return pixels_.data();
}

void Mandelbrot::color(int x, int y, int iterations)
{
    uint8_t p = static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * iterations / max_iterations_);
    int pixel_position = (y * width_ + x) * 3;
    pixels_[pixel_position + 0] = p;
    pixels_[pixel_position + 1] = p;
    pixels_[pixel_position + 2] = p;
}

}  // namespace fractal
