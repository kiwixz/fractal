#include "mandelbrot.h"
#include "palette.h"
#include <spdlog/spdlog.h>
#include <chrono>
#include <cmath>

namespace fractal {
namespace {

constexpr int bailout = 128;

}  // namespace


Mandelbrot::Mandelbrot(int width, int height)
{
    resize(width, height);
}

void Mandelbrot::resize(int width, int height)
{
    width_ = width;
    height_ = height;
    pixels_.resize(width * height);
}

uint32_t const* Mandelbrot::generate(float x, float y, float zoom)
{
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start = Clock::now();

    float y_size = 1 / zoom;
    float x_size = y_size * width_ / height_;
    float x_min = x - x_size / 2;
    float y_min = y - y_size / 2;
    float dx = x_size / width_;
    float dy = y_size / height_;

    int max_iterations = static_cast<int>(512 * std::sqrt(zoom));
    auto add_work = [&](int from_y, int to_y) {
        futures_.push(thread_pool_->submit([=] {
            for (int pixel_y = from_y; pixel_y < to_y; ++pixel_y) {
                for (int pixel_x = 0; pixel_x < width_; ++pixel_x) {
                    float real_x = x_min + dx * pixel_x;
                    float real_y = y_min + dy * pixel_y;
                    float x = 0;
                    float y = 0;

                    int iterations = 0;
                    while (iterations < max_iterations && x * x + y * y <= bailout) {
                        float x_tmp = x * x - y * y + real_x;
                        float y_tmp = 2 * x * y + real_y;
                        if (x_tmp == x && y_tmp == y) {
                            iterations = max_iterations;
                            break;
                        }

                        x = x_tmp;
                        y = y_tmp;
                        ++iterations;
                    }
                    pixels_[pixel_y * width_ + pixel_x] = color(x, y, iterations, max_iterations);
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

uint32_t Mandelbrot::color(float x, float y, int iterations, int max_iterations)
{
    if (iterations == max_iterations)
        return 0xff000000;

    float log_log_sqrt_zn = std::log(std::log(x * x + y * y)) - std::log(2.f);  // log(log(sqrt(x))) == log(log(x)) - log(2)
    float c = iterations - 1.28f + (std::log(std::log(bailout)) - log_log_sqrt_zn) / std::log(2.f);
    float index = std::fmod((std::log(c / 64 + 1) / std::log(2.f) + 0.45f), 1.) * palette.size();
    int index_int = static_cast<int>(index);

    // interpolate a and b
    uint32_t a = palette[index_int % palette.size()];
    uint32_t b = palette[(index_int + 1) % palette.size()];

    float b_k = index - index_int;
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

}  // namespace fractal
