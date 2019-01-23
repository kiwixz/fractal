#include <GLFW/glfw3.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <tinyfiledialogs.h>
#include <filesystem>
#include <memory>
#include <stdexcept>


#ifdef _WIN32
#    pragma comment(lib, "opengl32")
#endif


namespace fractal {

namespace {

struct Glfw {
    Glfw()
    {
        if (!glfwInit())
            throw std::runtime_error{"could not initialize glfw"};
    }

    ~Glfw()
    {
        glfwTerminate();
    }
};


void loop()
{
    Glfw glfw;

    GLFWwindow* window = glfwCreateWindow(1600, 900, "fractal", nullptr, nullptr);
    if (!window)
        throw std::runtime_error{"could not create window"};
    std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> window_raii{window, glfwDestroyWindow};

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int /*argc*/, char** /*argv*/)
{
    std::string log_file_path = (std::filesystem::temp_directory_path() / "fractal.log").string();
    auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file_path, true);
    auto logger = std::make_shared<spdlog::logger>("", spdlog::sinks_init_list{console_sink, file_sink});
    try {
        spdlog::register_logger(logger);
#ifdef _DEBUG
        spdlog::set_level(spdlog::level::debug);
#endif
        spdlog::set_pattern("%^[%H:%M:%S.%f][%t][%l]%$ %v");
        logger->info("starting...");
        loop();
        logger->info("terminating...");
        spdlog::drop_all();
        return 0;
    }
    catch (const std::exception& ex) {
        logger->critical("critical exception: {}", ex.what());
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
