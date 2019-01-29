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
    int palette_size = max_iterations + 1;
    palette_.resize(palette_size);

    struct Color {
        float p, h, s, v;
    };
    constexpr std::array colors = {Color{0.f, 240.f, 1.f, .5f},
                                   Color{.18f, 200.f, .5f, 1.f},
                                   Color{.2f, 200.f, 0.f, 1.f},
                                   Color{.2f, 33.f, 0.f, 1.f},
                                   Color{.3f, 33.f, 1.f, 1.f},
                                   Color{.5f, 33.f, 1.f, 0.f},
                                   Color{1.f, 33.f, 1.f, 0.f}};

    for (int i = 0; i < palette_size; ++i) {
        float p = static_cast<float>(i) / palette_size;

        auto it = std::find_if(colors.begin(), colors.end(), [&](Color const& color) {
            return color.p > p;
        });
        Color const& b = *it;
        Color const& a = *--it;

        float b_k = (p - a.p) / (b.p - a.p);
        float a_k = 1 - b_k;

        palette_[i] = hsv_to_bgr(a.h * a_k + b.h * b_k,
                                 a.s * a_k + b.s * b_k,
                                 a.v * a_k + b.v * b_k);
    }
}

uint32_t* Mandelbrot::generate()
{
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start = Clock::now();

    float dx = (x_max_ - x_min_) / width_;
    float dy = (y_max_ - y_min_) / height_;

    auto add_work = [&](int from_y, int to_y) {
        futures_.push(thread_pool_->submit([this, &dx, &dy, from_y, to_y] {
            for (int pixel_y = from_y; pixel_y < to_y; ++pixel_y) {
                for (int pixel_x = 0; pixel_x < width_; ++pixel_x) {
                    float real_x = x_min_ + dx * pixel_x;
                    float real_y = y_min_ + dy * pixel_y;
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
    float nu = std::log(log_zn / std::log(2.f)) / std::log(2.f);

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

    h = std::fmod(h, 360.f) / 60;
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
