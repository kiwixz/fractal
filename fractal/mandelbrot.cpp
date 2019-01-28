#include "mandelbrot.h"
#include <spdlog/spdlog.h>
#include <chrono>

namespace fractal {

Mandelbrot::Mandelbrot(int width, int height, int max_iterations)
{
    resize(width, height);
    set_max_iterations(max_iterations);
}

void Mandelbrot::resize(int width, int height)
{
    width_ = width;
    height_ = height;
    pixels_.resize(width * height);
}

void Mandelbrot::set_max_iterations(int max_iterations)
{
    max_iterations_ = max_iterations;
    palette_.resize(max_iterations + 1);
    for (int i = 0; i <= max_iterations; ++i) {
        palette_[i] = hsv_to_bgr(100.f * i / max_iterations, 1.f, 1.f);
    }
}

uint32_t* Mandelbrot::generate()
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
            pixels_[pixel_y * width_ + pixel_x] = color(iterations);
        }

    Clock::time_point end = Clock::now();
    spdlog::info("[mandelbrot] generated in {}ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    return pixels_.data();
}

uint32_t Mandelbrot::color(int iterations)
{
    return palette_[iterations];
}

uint32_t Mandelbrot::hsv_to_bgr(float h, float s, float v)
{
    float r, g, b;

    h /= 60;
    int i = floor(h);
    float f = h - i;
    float p = v * (1 - s);
    float q = v * (1 - s * f);
    float t = v * (1 - s * (1 - f));

    switch (i) {
    case 0:
        r = v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = t;
        g = p;
        b = v;
        break;
    case 5:
        r = v;
        g = p;
        b = q;
        break;
    default:
        break;
    }

    return static_cast<int>(r * 255) << 16
           | static_cast<int>(g * 255) << 8
           | static_cast<int>(b * 255);
}

}  // namespace fractal
