#pragma once

#include <vector>

namespace fractal {

struct Palette {
    Palette();

    uint32_t get(double t);

private:
    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    std::vector<Color> generated_;
};

}  // namespace fractal
