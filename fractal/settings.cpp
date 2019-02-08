#include "settings.h"
#include "fmt/printf.h"
#include <cxxopts.hpp>

namespace fractal {

Settings::Settings(int argc, char** argv)
{
    cxxopts::Options options{"fractal"};
    // clang-format off
    options.add_options()
        ("h,help", "Show help", cxxopts::value(help))
        ("W,width", "Render's width", cxxopts::value(width)->default_value("999"))
        ("H,height", "Render's height", cxxopts::value(height)->default_value("888"))
        ("x", "Horizontal center of render", cxxopts::value(x)->default_value("-0.75"))
        ("y", "Vertical center of render", cxxopts::value(y)->default_value("0"))
        ("z,zoom", "Initial zoom (unit based on render's height)", cxxopts::value(zoom)->default_value("0.42"))
        ("s,speed", "Zoom per second", cxxopts::value(zoom_speed))
        ;
    options.add_options("file output")
        ("o,out", "File to output render instead of displaying it", cxxopts::value(output_file))
        ("fps", "Frames per second", cxxopts::value(fps)->default_value("0"))
        ("d,duration", "Duration in seconds", cxxopts::value(duration)->default_value("10"))
        ;
    // clang-format on

    cxxopts::ParseResult parse = options.parse(argc, argv);
    if (help)
        fmt::print(options.help());
    if (argc != 1)
        throw std::runtime_error{"too much arguments"};
}

}  // namespace fractal
