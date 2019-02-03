#pragma once

namespace fractal::settings {

struct Settings {
    int width;
    int height;
    double x;
    double y;
    double initial_zoom;
    double zoom_speed;
};

bool parse(int argc, char** argv);  ///< Returns true if should exit.
Settings const& get();

}  // namespace fractal::settings
