#include "render_video.h"
#include "mandelbrot.h"
#include "scope_exit.h"
#include <x265.h>

namespace fractal {

void render_video(Settings const& settings)
{
    static ScopeExit x265_handle{[&] {
        x265_cleanup();
    }};

    (void)settings;

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
}

}  // namespace fractal
