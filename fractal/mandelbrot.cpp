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

uint32_t const* Mandelbrot::generate(double x_center, double y_center, double zoom)
{
    double y_size = 1 / zoom;
    double x_size = y_size * width_ / height_;
    double x_min = x_center - x_size / 2;
    double y_max = y_center + y_size / 2;
    double dx = x_size / width_;
    double dy = y_size / height_;

    int max_iterations = static_cast<int>(512 * std::pow(zoom, 0.1));
    auto add_work = [&](int from_y, int to_y) {
        futures_.push(thread_pool_->submit([=, this] {
            for (int pixel_y = from_y; pixel_y < to_y; ++pixel_y) {
                for (int pixel_x = 0; pixel_x < width_; ++pixel_x) {
                    double real_x = x_min + dx * pixel_x;
                    double real_y = y_max - dy * pixel_y;
                    double x = 0;
                    double y = 0;

                    int iterations = 0;
                    while (iterations < max_iterations && x * x + y * y <= bailout) {
                        double x_tmp = x * x - y * y + real_x;
                        double y_tmp = 2 * x * y + real_y;
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

    return pixels_.data();
}

uint32_t Mandelbrot::color(double x, double y, int iterations, int max_iterations)
{
    if (iterations == max_iterations)
        return 0xff000000;

    // precompute constants, constexpr would have been nice
    static const double log_log_bailout = std::log(std::log(bailout));
    static const double log_2 = std::log(2.);
    static const double log_2_inv = 1 / log_2;

    double log_log_sqrt_zn = std::log(std::log(x * x + y * y)) - log_2;  // log(log(sqrt(x))) == log(log(x)) - log(2)
    double c = iterations - 1.28 + (log_log_bailout - log_log_sqrt_zn) * log_2_inv;
    return palette_.get(std::log(c / 64 + 1) * log_2_inv - .1);
}

}  // namespace fractal
