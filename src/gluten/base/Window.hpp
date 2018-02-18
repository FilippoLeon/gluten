#pragma once

#include <string>
#include <stdexcept>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gluten/base/Color.hpp"

namespace gluten::base {

class Window {
public:
    Window(int width, int height, std::string title = "") {
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if ( window == nullptr ) {
            throw std::runtime_error("Failed to create window.");
        }
        glfwMakeContextCurrent(window);

        glfwSetWindowUserPointer(window, this);

        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD!");
        }

        using namespace std::placeholders;

        // TODO: resize correct viewport in case of multiple windows.
        glfwSetFramebufferSizeCallback(window, &Window::FramebufferSizeCallback);

        FramebufferSizeCallback(window, width, height);

        glEnable(GL_DEPTH_TEST);
    }

    ~Window() {
        glfwDestroyWindow(window);
    }

    inline void ProcessInput() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    inline bool ShouldClose() {
        return glfwWindowShouldClose(window);
    }

    inline void SwapBuffers() {
        glfwSwapBuffers(window);
    }

    inline void SetClearColor(Color color) {
        clearColor = color;
    }

    inline void Clear(bool depth = true) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | (depth ? GL_DEPTH_BUFFER_BIT : 0));
    }

    int Width() const { return width;  }
    int Height() const { return height; }

    GLFWwindow * get() const {
        return window;
    }
private:
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        Window *w_p = static_cast<Window*>(glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
        w_p->width = width;
        w_p->height = height;
    }

    int width, height;

    GLFWwindow * window;

    Color clearColor;
};

}