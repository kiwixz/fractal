#pragma once

#include "settings.h"
#include <x265.h>
#include <functional>
#include <memory>
#include <vector>

namespace fractal {

struct Encoder {
    using OutputCallback = std::function<void(std::byte const*, int)>;

    Encoder(Settings const& settings, OutputCallback output_callback);

    void encode_rgb(uint32_t const* pixels);
    void finish();

private:
    x265_param param_;
    std::unique_ptr<x265_encoder, void (*)(x265_encoder*)> encoder_;
    int nr_pixels_;
    std::vector<uint8_t> yuv_;
    x265_picture picture_;
    OutputCallback output_callback_;

    void rgb_to_yuv444_bt709(uint32_t const* pixels);
};

}  // namespace fractal
