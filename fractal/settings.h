#pragma once

namespace fractal::settings {

struct Settings {
    double x;
    double y;
    double zoom;
};

bool parse(int argc, char** argv);  ///< Returns true if should exit.
Settings const& get();

}  // namespace fractal::settings
