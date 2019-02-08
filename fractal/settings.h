#pragma once

#include <string>

namespace fractal {

struct Settings {
    Settings(int argc, char** argv);  ///< Also show help when appropriate.

    bool help = false;
    int width;
    int height;
    double x;
    double y;
    double zoom;
    double zoom_speed = 0;
    std::string output_file;
    int fps;
    int duration;
};

}  // namespace fractal
