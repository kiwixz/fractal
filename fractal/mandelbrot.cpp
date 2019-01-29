#include "mandelbrot.h"
#include <spdlog/spdlog.h>
#include <chrono>
#include <cmath>

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
    palette_.resize(max_iterations_ + 1);
    for (int i = 0; i <= max_iterations_; ++i)
        palette_[i] = hsv_to_bgr(240 + 120.f * i / max_iterations_, 1.f, 1.f);
}

uint32_t* Mandelbrot::generate()
{
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start = Clock::now();

    float dx = x_size_ / width_;
    float dy = y_size_ / height_;

    auto add_work = [&](int from_y, int to_y) {
        futures_.push(thread_pool_->submit([this, &dx, &dy, from_y, to_y] {
            for (int pixel_y = from_y; pixel_y < to_y; ++pixel_y) {
                for (int pixel_x = 0; pixel_x < width_; ++pixel_x) {
                    float real_x = x_offset_ + dx * pixel_x;
                    float real_y = y_offset_ + dy * pixel_y;
                    float x = 0;
                    float y = 0;

                    int iterations = 0;
                    while (iterations < max_iterations_ && x * x + y * y <= 2 * 2) {
                        float x_tmp = x * x - y * y + real_x;
                        float y_tmp = 2 * x * y + real_y;
                        if (x_tmp == x && y_tmp == y) {
                            iterations = max_iterations_;
                            break;
                        }

                        x = x_tmp;
                        y = y_tmp;
                        ++iterations;
                    }
                    pixels_[pixel_y * width_ + pixel_x] = color(x, y, iterations);
                }
            }
        }));
    };

    // we distribute work by divisions for a better handling of unbalanced pictures
    constexpr int work_divisions = 8;
    int division_size = height_ / work_divisions;
    int from_y = 0;
    int nr_works = static_cast<int>(thread_pool_->size());
    for (int work = 0; work < nr_works; ++work) {
        int to_y = from_y + (division_size - from_y) / (nr_works - work);
        for (int division = 0; division < work_divisions; ++division)
            add_work(division * division_size + from_y, division * division_size + to_y);
        from_y = to_y;
    }

    int work_done = division_size * work_divisions;
    if (work_done < height_)
        add_work(work_done, height_);

    while (!futures_.empty()) {
        futures_.front().get();
        futures_.pop();
    }

    Clock::time_point end = Clock::now();
    spdlog::info("[mandelbrot] generated in {}ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    return pixels_.data();
}

uint32_t Mandelbrot::color(float x, float y, int iterations)
{
    if (iterations == max_iterations_)
        return palette_[max_iterations_];

    float log_zn = std::log(x * x + y * y) / 2;
    float nu = std::log(log_zn / std::log(2)) / std::log(2);

    float smooth_iterations = iterations + 1 - nu;
    int smooth_iterations_int = static_cast<int>(smooth_iterations);

    // interpolate a and b
    uint32_t a = palette_[smooth_iterations_int];
    uint32_t b = palette_[smooth_iterations_int + 1];

    float b_k = smooth_iterations - smooth_iterations_int;
    float a_k = 1 - b_k;

    float a_r = ((a & 0x00ff0000) >> 16) * a_k;
    float a_g = ((a & 0x0000ff00) >> 8) * a_k;
    float a_b = (a & 0xff) * a_k;

    float b_r = ((b & 0x00ff0000) >> 16) * b_k;
    float b_g = ((b & 0x0000ff00) >> 8) * b_k;
    float b_b = (b & 0xff) * b_k;

    return 0xff000000
           | static_cast<int>(a_r + b_r) << 16
           | static_cast<int>(a_g + b_g) << 8
           | static_cast<int>(a_b + b_b);
}

uint32_t Mandelbrot::hsv_to_bgr(float h, float s, float v)
{
    float r = 0;
    float g = 0;
    float b = 0;

    h /= 60;
    int i = static_cast<int>(h);
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
