#include "render_video.h"
#include "encoder.h"
#include "mandelbrot.h"
#include <fstream>

namespace fractal {

void render_video(Settings const& settings)
{
    std::ofstream ofs{settings.output_file, std::ios::binary};

    Encoder encoder{settings, [&](std::byte const* data, int size) {
                        ofs.write(reinterpret_cast<char const*>(data), size);
                    }};

    int nr_frames = settings.duration * settings.fps;

    // tmp
    std::vector<uint32_t> pixels_v(settings.width * settings.height);
    for (int y = 0; y < settings.height; ++y)
        for (int x = 0; x < settings.width; ++x)
            pixels_v[y * settings.width + x] = 0xffff0000
                                               + ((255 * x / settings.width) << 8)
                                               + (255 * y / settings.height);

    for (int frame = 0; frame < nr_frames; ++frame)
        encoder.encode_rgb(pixels_v.data());
    encoder.finish();
}

}  // namespace fractal
