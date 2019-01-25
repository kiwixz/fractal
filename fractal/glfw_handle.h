#pragma once

#include <memory>

namespace fractal {

struct GlfwHandle {
    GlfwHandle();

private:
    std::shared_ptr<void> instance_;

    static std::shared_ptr<void> get_instance();
};

}  // namespace fractal
