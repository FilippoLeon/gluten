#pragma once

#include <stdexcept>
#ifdef EXPERIMENTAL_FILESYSTEM
    #include <experimental/filesystem>
#else
    #include <filesystem>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gluten::base {

namespace filesystem = std::experimental::filesystem;

class Application {
public:
    Application() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    }

    void PollEvents() {
        glfwPollEvents();
    }

    std::string GlutenRoot(std::string postfix) {

        return (root_dir / filesystem::path(postfix)).string();
    }

    ~Application() {
        glfwTerminate();
    }

private:
    static const filesystem::path root_dir;
};

}