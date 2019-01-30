#pragma once

#include "settings.h"
#include "thread_pool.h"
#include <cstdint>
#include <queue>
#include <vector>

namespace fractal {

struct Mandelbrot {
    Mandelbrot() = default;
    Mandelbrot(int width, int height, int max_iterations);

    void resize(int width, int height);
    void set_max_iterations(int max_iterations);
    uint32_t const* generate(float x, float y, float zoom);  ///< pointer may be invalidated after any interaction with this object

private:
    std::vector<uint32_t> pixels_;
    std::vector<uint32_t> palette_;
    int width_;
    int height_;
    int max_iterations_;
    std::unique_ptr<ThreadPool> thread_pool_ =
            std::make_unique<ThreadPool>(std::thread::hardware_concurrency());
    std::queue<std::future<void>> futures_;

    uint32_t color(float x, float y, int iterations, int max_iterations);
    uint32_t hsv_to_bgr(float h, float s, float v);
};

}  // namespace fractal
