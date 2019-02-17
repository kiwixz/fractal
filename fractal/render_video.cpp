#include "render_video.h"
#include "mandelbrot.h"
#include "scope_exit.h"
#include <x265.h>

namespace fractal {
namespace {

std::unique_ptr<x265_encoder, void (*)(x265_encoder*)> initialize_encoder(Settings const& settings)
{
    x265_param param;
    if (x265_param_default_preset(&param, "1", nullptr)
        || x265_param_parse(&param, "colorprim", "bt709")
        || x265_param_parse(&param, "transfer", "bt709")
        || x265_param_parse(&param, "colormatrix", "bt709"))
        throw std::runtime_error{"could not select x265 params"};
    param.sourceWidth = settings.width;
    param.sourceHeight = settings.height;
    param.fpsNum = settings.fps;
    param.fpsDenom = 1;
    param.internalCsp = X265_CSP_I444;
    param.bLossless = true;

    std::unique_ptr<x265_encoder, void (*)(x265_encoder*)> encoder = {x265_encoder_open(&param), &x265_encoder_close};
    if (!encoder)
        throw std::runtime_error{"could not open x265 encoder"};
    return encoder;
}

void rgb_to_yuv444_bt709(uint32_t const* pixels, uint8_t* pixels_yuv, int nr_pixels)
{
    for (int i = 0; i < nr_pixels; ++i) {
        int r = pixels[i] >> 16 & 0xff;
        int g = pixels[i] >> 8 & 0xff;
        int b = pixels[i] & 0xff;
        pixels_yuv[i] = 0;
        pixels_yuv[i + nr_pixels] = 0;
        pixels_yuv[i + nr_pixels * 2] = 0;
    }
}

}  // namespace


void render_video(Settings const& settings)
{
    static ScopeExit x265_handle{[&] {
        x265_cleanup();
    }};

    std::unique_ptr<x265_encoder, void (*)(x265_encoder*)> encoder = initialize_encoder(settings);
    x265_param param;
    x265_encoder_parameters(&*encoder, &param);

    // tmp
    std::vector<uint32_t> pixels_v(settings.width * settings.height);
    std::fill(pixels_v.begin(), pixels_v.end(), 0xff880000);

    int nr_pixels = settings.width * settings.height;
    std::vector<uint8_t> pixels_yuv(nr_pixels * 3);
    x265_picture picture;
    x265_picture_init(&param, &picture);
    picture.planes[0] = pixels_yuv.data();
    picture.planes[0] = pixels_yuv.data() + nr_pixels;
    picture.planes[0] = pixels_yuv.data() + nr_pixels * 2;

    int nr_frames = settings.duration * settings.fps;

    nr_frames = 120;

    for (int frame = 0; frame < nr_frames; ++frame) {
        uint32_t const* pixels = pixels_v.data();
        rgb_to_yuv444_bt709(pixels, pixels_yuv.data(), nr_pixels);
        // x265_encoder_encode(&*encoder, );
    }
}

}  // namespace fractal
