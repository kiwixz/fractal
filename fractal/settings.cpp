#include "settings.h"
#include "fmt/printf.h"
#include <cxxopts.hpp>

namespace fractal::settings {
namespace {

Settings& get_mutable()
{
    static Settings settings;
    return settings;
}

}  // namespace


bool parse(int argc, char** argv)
{
    Settings& settings = get_mutable();

    cxxopts::Options options{"fractal"};
    // clang-format off
    options.add_options()
        ("h,help", "Show help")
        ("W,width", "Render's width", cxxopts::value(settings.width)->default_value("999"))
        ("H,height", "Render's height", cxxopts::value(settings.height)->default_value("888"))
        ("x", "Horizontal center of render", cxxopts::value(settings.x)->default_value("-0.75"))
        ("y", "Vertical center of render", cxxopts::value(settings.y)->default_value("0"))
        ("z,zoom", "Initial zoom (unit based on render's height)", cxxopts::value(settings.initial_zoom)->default_value("0.42"))
        ("s,speed", "Speed of zooming", cxxopts::value(settings.zoom_speed)->default_value("0"))
        ;
    // clang-format on

    cxxopts::ParseResult parse = options.parse(argc, argv);
    if (parse.count("help")) {
        fmt::print(options.help());
        return true;
    }

    if (argc != 1)
        throw std::runtime_error{"too much arguments"};

    return false;
}

Settings const& get()
{
    return get_mutable();
}

}  // namespace fractal::settings
