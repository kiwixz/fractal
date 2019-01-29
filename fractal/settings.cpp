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
        ("x,x_min", "Minimum x", cxxopts::value(settings.x_min)->default_value("-2.1"))
        ("X,x_max", "Maximum x", cxxopts::value(settings.x_max)->default_value(" 0.6"))
        ("y,y_min", "Minimum y", cxxopts::value(settings.y_min)->default_value("-1.2"))
        ("Y,y_max", "Maximum y", cxxopts::value(settings.y_max)->default_value(" 1.2"))
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
