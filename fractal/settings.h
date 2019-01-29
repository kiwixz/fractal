#pragma once

namespace fractal::settings {

struct Settings {
    float x_min;
    float x_max;
    float y_min;
    float y_max;
};

bool parse(int argc, char** argv);  ///< Returns true if should exit.
Settings const& get();

}  // namespace fractal::settings
