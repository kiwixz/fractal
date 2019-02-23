#include "render_video.h"
#include "encoder.h"
#include "mandelbrot.h"
#include <spdlog/spdlog.h>
#include <fstream>

namespace fractal {

void render_video(Settings const& settings)
{
    std::ofstream ofs{settings.output_file, std::ios::binary};
    Encoder encoder{settings, [&](std::byte const* data, int size) {
                        ofs.write(reinterpret_cast<char const*>(data), size);
                    }};

    Mandelbrot mandelbrot{settings.width, settings.height};
    double zoom = settings.zoom;

    int nr_frames = settings.duration * settings.fps;
    for (int frame = 0; frame < nr_frames; ++frame) {
        encoder.encode_rgb(mandelbrot.generate(settings.x, settings.y, zoom));
        spdlog::info("[render_video] frame {}/{}", frame, nr_frames);
        zoom += zoom * settings.zoom_speed / settings.fps;
    }
    encoder.finish();
}

}  // namespace fractal
