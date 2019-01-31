#pragma once

#include "settings.h"
#include "thread_pool.h"
#include <cstdint>
#include <queue>
#include <vector>

namespace fractal {

struct Mandelbrot {
    Mandelbrot() = default;
    Mandelbrot(int width, int height);

    void resize(int width, int height);
    uint32_t const* generate(double x, double y, double zoom);  ///< pointer may be invalidated after any interaction with this object

private:
    std::vector<uint32_t> pixels_;
    std::vector<uint32_t> palette_;
    int width_;
    int height_;
    std::unique_ptr<ThreadPool> thread_pool_ =
            std::make_unique<ThreadPool>(std::thread::hardware_concurrency());
    std::queue<std::future<void>> futures_;

    uint32_t color(double x, double y, int iterations, int max_iterations);
};

}  // namespace fractal
