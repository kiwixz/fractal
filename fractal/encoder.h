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

    void set_yuv_from_rgb(uint32_t const* pixels);  ///< Convert RBG pixels to YUV444 (BT.709)

    template <typename T, typename... Args>
    auto forward_encoder_output(T&& callable, Args&&... args);
};


template <typename T, typename... Args>
auto Encoder::forward_encoder_output(T&& callable, Args&&... args)
{
    x265_nal* nals;
    uint32_t nr_nals;
    auto ret = std::forward<T>(callable)(encoder_.get(), &nals, &nr_nals, std::forward<Args>(args)...);
    for (unsigned i = 0; i < nr_nals; ++i) {
        x265_nal const& nal = nals[i];
        output_callback_(reinterpret_cast<std::byte const*>(nal.payload), nal.sizeBytes);
    }
    return ret;
}

}  // namespace fractal
