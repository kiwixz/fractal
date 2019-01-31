#include "palette.h"
#include <array>
#include <cmath>

namespace fractal {

Palette::Palette()
{
    constexpr std::array colors = {
            Color{14, 11, 82},
            Color{14, 60, 165},
            Color{78, 154, 220},
            Color{176, 229, 255},
            Color{249, 253, 239},
            Color{249, 229, 96},
            Color{249, 181, 15},
            Color{217, 108, 3},
            Color{134, 32, 13},
            Color{57, 4, 35},
            Color{14, 11, 82},
    };

    constexpr int palette_size = 4096;
    generated_.reserve(palette_size);
    for (int i = 0; i < palette_size; ++i) {
        double t = i / static_cast<double>(palette_size);
        double index_f = t * (colors.size() - 1);
        int index = static_cast<int>(index_f);

        Color const& a = colors[index];
        Color const& b = colors[index + 1];
        double b_k = index_f - index;
        double a_k = 1 - b_k;

        generated_.push_back({static_cast<uint8_t>(std::lround(a.r * a_k + b.r * b_k)),
                              static_cast<uint8_t>(std::lround(a.g * a_k + b.g * b_k)),
                              static_cast<uint8_t>(std::lround(a.b * a_k + b.b * b_k))});
    }
}

uint32_t Palette::get(double t)
{
    double index_f = std::fmod(t, 1.) * generated_.size();
    int index = static_cast<int>(index_f);

    // interpolate a and b
    Color const& a = generated_[index % generated_.size()];
    Color const& b = generated_[(index + 1) % generated_.size()];
    double b_k = index_f - index;
    double a_k = 1 - b_k;

    return 0xff000000
           | static_cast<int>(std::lround(a.r * a_k + b.r * b_k)) << 16
           | static_cast<int>(std::lround(a.g * a_k + b.g * b_k)) << 8
           | static_cast<int>(std::lround(a.b * a_k + b.b * b_k));
}

}  // namespace fractal
