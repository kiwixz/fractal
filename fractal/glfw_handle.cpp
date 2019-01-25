#include "glfw_handle.h"
#include <glad/glfw.h>
#include <stdexcept>

namespace fractal
{

GlfwHandle::GlfwHandle() :
    instance_{get_instance()}
{}

std::shared_ptr<void> GlfwHandle::get_instance()
{
    static std::weak_ptr<void> old_instance;

    if (std::shared_ptr<void> instance = old_instance.lock())
        return instance;

    if (!glfwInit())
        throw std::runtime_error{"could not initialize glfw"};
    std::shared_ptr<void> instance{nullptr, [](auto) {
                                       glfwTerminate();
                                   }};
    old_instance = instance;
    return instance;
}

}  // namespace fractal
