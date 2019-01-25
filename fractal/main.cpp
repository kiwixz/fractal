#include <spdlog/spdlog.h>  // workaround to be removed after merge of https://github.com/Microsoft/vcpkg/pull/5175

#include "main_window.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <tinyfiledialogs.h>
#include <filesystem>
#include <stdexcept>


namespace fractal {
namespace {

void main_core()
{
    MainWindow main_window;
    main_window.loop();
}

int main(int /*argc*/, char** /*argv*/)
{
    try {
        std::string log_file_path = (std::filesystem::temp_directory_path() / "fractal.log").string();
        auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file_path, true);
        auto logger = std::make_shared<spdlog::logger>("", spdlog::sinks_init_list{console_sink, file_sink});
        spdlog::set_default_logger(logger);
#ifdef _DEBUG
        spdlog::set_level(spdlog::level::debug);
#endif
        spdlog::set_pattern("%^[%H:%M:%S.%f][%t][%l]%$ %v");
        spdlog::info("starting...");
        main_core();
        spdlog::info("terminating...");
        spdlog::drop_all();
        return 0;
    }
    catch (std::exception const& ex) {
        spdlog::critical("critical exception: {}", ex.what());
        tinyfd_messageBox("Critical Exception", ex.what(), "ok", "error", 0);
        return 1;
    }
}

}  // namespace
}  // namespace fractal


int main(int argc, char** argv)
{
    fractal::main(argc, argv);
}
