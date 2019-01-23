#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>


#if _WIN32
#    pragma comment(lib, "opengl32")
#endif


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


int main()
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
