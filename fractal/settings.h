#pragma once

namespace fractal::settings {

struct Settings {
    float x;
    float y;
    float zoom;
};

bool parse(int argc, char** argv);  ///< Returns true if should exit.
Settings const& get();

}  // namespace fractal::settings
