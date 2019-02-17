#include "encoder.h"
#include "scope_exit.h"
#include <stdexcept>

namespace fractal {

Encoder::Encoder(Settings const& settings, OutputCallback output_callback) :
    encoder_{nullptr, &x265_encoder_close},
    nr_pixels_{settings.width * settings.height},
    output_callback_{output_callback}
{
    static ScopeExit x265_handle{[&] {
        x265_cleanup();
    }};

    if (x265_param_default_preset(&param_, "0", nullptr)
        || x265_param_parse(&param_, "colorprim", "bt709")
        || x265_param_parse(&param_, "transfer", "bt709")
        || x265_param_parse(&param_, "colormatrix", "bt709"))
        throw std::runtime_error{"could not select x265 params"};
    param_.sourceWidth = settings.width;
    param_.sourceHeight = settings.height;
    param_.fpsNum = settings.fps;
    param_.fpsDenom = 1;
    param_.internalCsp = X265_CSP_I444;
    param_.bLossless = true;

    encoder_.reset(x265_encoder_open(&param_));
    if (!encoder_)
        throw std::runtime_error{"could not open x265 encoder"};

    forward_encoder_output(&x265_encoder_headers);

    yuv_.resize(nr_pixels_ * 3);
    x265_picture_init(&param_, &picture_);
    picture_.planes[0] = yuv_.data();
    picture_.planes[1] = yuv_.data() + nr_pixels_;
    picture_.planes[2] = yuv_.data() + nr_pixels_ * 2;
}

void Encoder::encode_rgb(uint32_t const* pixels)
{
    set_yuv_from_rgb(pixels);
    forward_encoder_output(&x265_encoder_encode, &picture_, nullptr);
}

void Encoder::finish()
{
    while (true)
        if (forward_encoder_output(&x265_encoder_encode, nullptr, nullptr) == 0)
            break;
}

void Encoder::set_yuv_from_rgb(uint32_t const* pixels)
{
    for (int i = 0; i < nr_pixels_; ++i) {
        int r = pixels[i] >> 16 & 0xff;
        int g = pixels[i] >> 8 & 0xff;
        int b = pixels[i] & 0xff;
        yuv_[i] = 0;
        yuv_[i + nr_pixels_] = 0;
        yuv_[i + nr_pixels_ * 2] = 0;
    }
}

}  // namespace fractal
