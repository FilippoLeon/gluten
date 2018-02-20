#pragma once

#include <stdexcept>
#ifdef EXPERIMENTAL_FILESYSTEM
    #include <experimental/filesystem>
#else
    #include <filesystem>
#endif

#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gluten/utils/accu.hpp"

namespace gluten::base {

namespace filesystem = std::experimental::filesystem;

class Application {
public:
    Application() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        last_update = std::chrono::high_resolution_clock::now();
    }

    void Update() {
        auto now = std::chrono::high_resolution_clock::now();
        elapsed = now - last_update;
        if ( elapsed_avg.Count() > avg_interval ) {
            elapsed_avg.Clear();
        }
        elapsed_avg.Add(elapsed);

        last_update = now;
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

    std::chrono::high_resolution_clock::duration elapsed;

    int avg_interval = 10;
    gluten::accu::Mean<std::chrono::high_resolution_clock::duration> elapsed_avg;
private:
    static const filesystem::path root_dir;

    std::chrono::high_resolution_clock::time_point last_update;
};

}